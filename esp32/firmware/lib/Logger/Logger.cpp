#include "Logger.h"



Logger::Logger(){
    // try with local house wifi first 20 times then try hotspot. if neither work then don't upload data
    // instead just save the number of times that the temperature was read, this will allow me
    // to see how long the data outage was for retrospectively.
    // when esp32 reconnects, it will try and read the address of the data and see if data was unsent
    // if so, it sends an error message to server telling it how long the outage was for so server can log it.
    Serial.begin(9600);
    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(SSID1,PWD1);
    int counter = 0;
    while(WiFi.status() != 3 && counter < 21){
        Serial.println(".");
        delay(250);
        counter++;
    }
    Serial.println("Trying Hotspot");
    if(WiFi.status() != 3){ // if not at home or home wifi not working
        Serial.println(".");
        WiFi.begin(SSID2,PWD2);
        counter = 0;
        while(WiFi.status() != 3 && counter < 21){
            delay(250);
            counter++;
        }
    }
    
    EEPROM.begin(EEPROM_SIZE);

    // update server with num fails if necessary.
    if(WiFi.status() == 3){
        int numFails = getNumFailedSends();
        if(numFails != 0)
            sendNumFails(numFails);
    }
    
    
}

void Logger::sendTempData(float temp){
    if(WiFi.status() == 3){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, "https://www.growhab.com/inputTempData");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String data = "password=" + String(DATA_PASSWORD) + "&temperature=" + String(temp);
        int respCode = http.POST(data);
        Serial.println(respCode);
        if(respCode != 200)
            failedSendHandler();
        else Serial.print("Successful ");
    }else{
        failedSendHandler();
    }
    Serial.println(temp);
}

void Logger::sendECData(float EC){
    if(WiFi.status() == 3){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, "https://www.growhab.com/inputECData");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String data = "password=" + String(DATA_PASSWORD) + "&EC=" + String(EC);
        int respCode = http.POST(data);
        Serial.println(respCode);
        if(respCode != 200)
            failedSendHandler();
        else Serial.print("Successful ");
    }else{
        failedSendHandler();
    }
    Serial.println(EC);
}

void Logger::failedSendHandler(){
    /*
    * this function increments the number of failed sends in the EEPROM
    */
   Serial.println(WiFi.status());
   int numFailed = EEPROM.read(0);
   numFailed++;
   EEPROM.write(0, numFailed);
   EEPROM.commit();
}

int Logger::getNumFailedSends(){
    // gets the number of failed data sends from EEPROM
    int num = EEPROM.read(0);
    if(num == 255) return 0;
    else return num;
}

void Logger::sendNumFails(int numFails){
    // updates the server when reconnected to wifi on how many failed sends there have been
    WiFiClient client;
    HTTPClient http;

    http.begin(client, "http://growhab.net/dataOutage/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String data = "password=" + String(DATA_PASSWORD) + "&failedSends=" + numFails;
    int resp = http.POST(data);
    if(resp == 200){
        resetFails();
    }
}
void Logger::resetFails(){
    EEPROM.write(0,0);
    EEPROM.commit();
}