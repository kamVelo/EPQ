int relayPin = 26;
bool pumped = false;
int prevPumpTime = 0;

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int R1= 1000; // Value of resistor for EC probe
int EC_Read = 12;
int ECPower = 14;

int Temp_Pin = 13;
OneWire oneWire(Temp_Pin);
DallasTemperature sensors(&oneWire);

float Temp_C; // Do not change
float Temp_F; // Do not change
float Temp1_Value = 0;
float Temp_Coef = 0.019; // You can leave as it is
/////////////////This part needs your attention during calibration only///////////////
float Calibration_PPM =1080 ; //Change to PPM reading measured with a separate meter
float K=-0.25; //You must change this constant once for your probe(see video)
float PPM_Con=0.5; //You must change this only if your meter uses a different factor
/////////////////////////////////////////////////////////////////////////////////////
//float CalibrationEC= (Calibration_PPM*2)/1000;
float CalibrationEC = 2.76;
float Temperature;
float EC;
float EC_at_25;
int ppm;
float A_to_D= 0;
float Vin= 3.3;
float Vdrop= 0;
float R_Water;
float Value=0;

void setup(){
  Serial.begin(9600);
  Serial.println("hello");
  pinMode(relayPin, OUTPUT);
  pinMode(relayPin, LOW);
  sensors.begin();
  pinMode(EC_Read,INPUT);
  pinMode(ECPower,OUTPUT);
  
  Serial.println(getEC());
}

void loop(){
  int currTime = millis();
  if(currTime-prevPumpTime >= 60*1000){
    float EC = getEC();
    Serial.println(EC);
    Serial.println("PUMPING");
    prevPumpTime = currTime;
    pumpBurst(2500);
  }
  delay(500);
  Serial.println(getEC());
}

void pumpBurst(int ms){
  digitalWrite(relayPin, HIGH);
  delay(ms);
  digitalWrite(relayPin, LOW);
}
float getEC()
{
  Temp_C = getTemp();
  Temp1_Value = Temp_C;
  Temperature = Temp_C;
  digitalWrite(ECPower,HIGH);
  A_to_D= analogRead(EC_Read);
  A_to_D= analogRead(EC_Read);
  digitalWrite(ECPower,LOW);
  Vdrop= (Vin*A_to_D) / 1024.0;
  R_Water = (Vdrop*R1) / (Vin-Vdrop);
  EC = 1000/ (R_Water*K);
  EC_at_25 = EC / (1+ Temp_Coef*(Temperature-25.0));
  ppm=(EC_at_25)*(PPM_Con*1000);
  return EC_at_25;
}

int getTemp(){
  sensors.requestTemperatures();
  Temp_C = sensors.getTempCByIndex(0);
  Temp_F = sensors.getTempFByIndex(0);
  return Temp_C;
}