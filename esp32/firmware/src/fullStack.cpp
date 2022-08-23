// Libraries:
#include "Temperature.h" // custom temperature library
#include "EC.h" // custom EC reading library
#include "waterLevel.h" // custom water level stasis library
#include "Secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <EEPROM.h>

#define EEPROM_SIZE 1

// wifi passwords:

const char * SSID1 = "SKY06DF3";
const char * PWD1 = HOME_WIFI_PASSWORD;
const char * SSID2 = "Ali Kamel's iPhone";
const char * PWD2 = HOTSPOT_PASSWORD;


// pin Nums:
int tempReadPin = 18;
int ECPowerPin = 14;
int ECReadPin = 34;
int ECRelayPin = 26;
int heaterRelayPin = 25;
int waterLevelReadPin = 35;
int waterRelay = 33;

// timing variables

float currentMillis; // stores current milliseconds since power up;
float tempDelay = 15000;

float lastTemp = 0; // stores the last time temperature read

float ECDelay = 300000;
float lastEC = 0; // last time EC read

float waterDelay = 900000;
float lastWater = 0; // last water level read

// set points:
float tempSetPoint = 25.0; // Ideal Temperature
float ecSetPoint = 2.8;
float waterSetpoint = 1700;

// objects:

Temperature temp(tempReadPin, tempSetPoint, heaterRelayPin);
//Logger logger = Logger();
EC ec(ECReadPin, ECPowerPin, ECRelayPin, ecSetPoint);
waterLevel water(waterLevelReadPin, waterRelay, waterSetpoint);

// function call enum:
enum functionCall{
  TemperatureCheck,
  ECCheck,
  waterLevelCheck,
  Nothing
};

// functions

void resetFails(){
    EEPROM.write(0,0);
    EEPROM.commit();
}
void sendNumFails(int numFails){
    // updates the server when reconnected to wifi on how many failed sends there have been
    WiFiClient client;
    HTTPClient http;

    http.begin(client, "http://wwww.growhab.net/dataOutage/");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String data = "password=" + String(DATA_PASSWORD) + "&failedSends=" + numFails;
    int resp = http.POST(data);
    if(resp == 200){
        resetFails();
    }
}

int getNumFailedSends(){
    // gets the number of failed data sends from EEPROM
    int num = EEPROM.read(0);
    if(num == 255) return 0;
    else return num;
}

void failedSendHandler(){
    /*
    * this function increments the number of failed sends in the EEPROM
    */
   Serial.println(WiFi.status());
   int numFailed = EEPROM.read(0);
   numFailed++;
   EEPROM.write(0, numFailed);
   EEPROM.commit();
}

void sendTempData(float temp){
    if(WiFi.status() == 3){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, "http://www.growhab.net/inputTempData");
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

void sendECData(float EC){
    if(WiFi.status() == 3){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, "http://www.growhab.net/inputElectroData");
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




functionCall whatStep(){
  /*
   * This function dictates what routine the esp32 should do
   * based on the current time.
   */

  currentMillis = millis();
  float tempDiff = currentMillis - lastTemp;
  float ECDiff = currentMillis -  lastEC;
  float waterDiff = currentMillis - lastWater;
  if(waterDiff >= waterDelay){
    lastWater = currentMillis;
    Serial.println("water");
    return waterLevelCheck;
  } else if(ECDiff >= ECDelay){
    lastEC = currentMillis;
    Serial.println("EC");
    return ECCheck;
  }else if(tempDiff >= tempDelay){
    lastTemp = currentMillis;
    Serial.println("temp");
    return TemperatureCheck;
  }
  else return Nothing;
}

float tempRoutine(){
  float temperature = temp.getTemp();
  temp.heat(temperature);
  sendTempData(temperature);
  return temperature;
}
float ECRoutine(float temperature){
  float ECReading = ec.getEC(temperature);
  ec.modulateEC(ECReading);
  sendECData(ECReading);
  return ECReading;
}



void setup() {
  Serial.begin(9600);
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
        Serial.println(".");
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




  float temperature = tempRoutine();
  ECRoutine(temperature);
}

void loop() {
  // use what step to get the function calls to run
  functionCall call = whatStep();
  if(call == TemperatureCheck){
    tempRoutine();
  }else if(call == ECCheck){
    float temperature = tempRoutine();
    ECRoutine(temperature);
  }else if(call == waterLevelCheck){
    float temperature = tempRoutine();
    ECRoutine(temperature);
    water.increase();
  }
}





