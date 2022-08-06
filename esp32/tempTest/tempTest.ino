#include <OneWire.h>
#include <DallasTemperature.h>
int tempPin = 14;
OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  sensors.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = getTemp();
  Serial.print("Temperature: ");
  Serial.println(temp);
  delay(5000);
}
int getTemp(){
  sensors.requestTemperatures();
  float Temp_C = sensors.getTempCByIndex(0);
  return Temp_C;
}
