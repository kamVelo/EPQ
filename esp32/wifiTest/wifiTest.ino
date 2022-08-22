#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char * SSID1 = "SKY06DF3";
const char *  PWD1 = "Btrbtr12";
int tempPin = 13;

void setup() {
  // put your setup code here, to run once:
  OneWire oneWire(tempPin);
  DallasTemperature dallasTemperature(&oneWire);  

  WiFi.begin(SSID1, PWD1);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}


void sendTempData(float temp){
    if(WiFi.status() == WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, "https://growhab.com/inputTempData");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String data = "password=" + String(DATA_PASSWORD) + "&temperature=" + String(temp);
        int respCode = http.POST(data);
        if(respCode != 200)
            Serial.println("failed");
        else Serial.println("succeeded");
    }else{
        Serial.println("failed");
    }
    
}

int getTemp(){
  sensors.requestTemperatures();
  Temp_C = sensors.getTempCByIndex(0);
  return Temp_C;
}
