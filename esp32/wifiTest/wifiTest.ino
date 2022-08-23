#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char * SSID1 = "SKY06DF3";
const char *  PWD1 = "VTNPMQXDNP";
int tempPin = 13;
OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);
void setup() {
  // put your setup code here, to run once:
  sensors.begin();
  Serial.begin(9600);
  WiFi.begin(SSID1, PWD1);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(WiFi.status());
  float temp = getTemp();
  Serial.println(temp);
  sendTempData(temp);
  delay(500);
}


void sendTempData(float temp){
  
    if(WiFi.status() == 3){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, "http://www.growhab.net/inputTempData");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String data = "password=theySpeakEnglishInWhat?&temperature=" + String(temp);
        int respCode = http.POST(data);
        Serial.println(respCode);
        http.end();
    }else{
        Serial.println("failed not connected");
    }
}

float getTemp(){
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}
