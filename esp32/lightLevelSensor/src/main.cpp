#include <Arduino.h>

int lightPin = 2;
int vIn = 3.3;

void setup() {
  pinMode(lightPin, INPUT);
  Serial.begin(9600);
}

float convertToLumens(int raw){
  float Vout = float(raw) * (vIn / float(4095));// Conversion analog to voltage
  float RLDR = (10000* (vIn - Vout))/Vout; // Conversion voltage to resistance
  int phys=500/(RLDR/1000); // Conversion resitance to lumen
  return phys;
}

void loop() {
  // put your main code here, to run repeatedly:
  int rawVal = analogRead(lightPin);
  float lux = convertToLumens(rawVal);

  Serial.print("Raw Sensor Reading: ");
  Serial.println(rawVal);
  Serial.print("In Lumens: ");
  Serial.println(lux);
  delay(500);
}


