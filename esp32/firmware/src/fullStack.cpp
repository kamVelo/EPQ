// Libraries:
#include "Temperature.h" // custom temperature library

// pin Nums:
int zeroCrossPin = 27;
int tempReadPin = 13;
int ECPowerPin = 14;
int ECReadPin = 12;
int firePin =  18;

// misc variables:
bool zeroCrossDetected = false; // keeps track of whether AC wave has crossed
float PIDVal = 0;
Temperature temp;

// timing variables

float currentMillis; // stores current milliseconds since power up;
float tempDelay = 6000;

float lastTemp = 0; // stores the last time temperature read


float setPoint = 25; // Ideal Temperature

// function call enum:
enum functionCall{
  TemperatureCheck,
  Nothing
};


OneWire oneWire(tempReadPin);
DallasTemperature sensors(&oneWire);

// functions

void IRAM_ATTR zeroCross(){ // IRAM_ATTR loads function into IRAM
  zeroCrossDetected = true;
}

functionCall whatStep(){
  /*
   * This function dictates what routine the esp32 should do
   * based on the current time.
   */

  currentMillis = millis();
  float tempDiff = currentMillis - lastTemp;


  if(tempDiff >= tempDelay){
    lastTemp = currentMillis;
    return TemperatureCheck;
  }
  else return Nothing;
}





void setup() {
  // setting up zeroCrossPin
  pinMode(zeroCrossPin, INPUT);
  
  attachInterrupt(zeroCrossPin, zeroCross, RISING); // run func on each change
  Serial.begin(9600);
  temp.begin(tempReadPin, 25.0, firePin);
  sensors.begin();
}

void loop() {
  // use what step to get the function calls to run
  functionCall call = whatStep();
  switch(call){
    case TemperatureCheck:
      sensors.requestTemperatures();
      float temp = sensors.getTempCByIndex(0);
      Serial.println(temp);
  } 
  
  if(zeroCrossDetected){
    
    temp.fireHeaterPulse(7400); // replace 0 with PID val when ready.
    zeroCrossDetected = false;
  } 
}
