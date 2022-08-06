// Libraries:
#include "temperature.h" // custom temperature library

// pin Nums:
int zeroCrossPin = 27;
int tempReadPin = 14;
int ECPowerPin = 12;
int ECReadPin = 13;
int firePin = 15;

// misc variables:
bool zeroCrossDetected = false; // keeps track of whether AC wave has crossed
float PIDVal = 0;

// timing variables
float prevMillis = 0; // stores previous milliseconds since power up 
float currentMillis; // stores current milliseconds since power up;
float tempDelay = 500;

float setPoint = 25; // Ideal Temperature


// functions
void IRAM_ATTR zeroCross(){ // IRAM_ATTR loads function into IRAM
  zeroCrossDetected = true;
}

String *whatStep(){
  /*
   * This function dictates what routine the esp32 should do
   * based on the current time.
   */
   String[5] functionCalls;;
   currentMillis = millis();
   diff = currentMillis - prevMillis;

   if(diff >= tempDelay){
    functionCalls[0] = "temperature";
   }
   
}





void setup() {
  // setting up zeroCrossPin
  pinMode(zeroCrossPin, INPUT);
  attachInterrupt(zeroCrossPin, zeroCross, CHANGE); // run func on each change

  // setting up temp read
  Temperature temp(tempReadPin, 25.0, firePin);
}

void loop() {
  // use what step to get the function calls to run
  String functionCalls = whatStep();
  for(int i = 0; i < functionsCall.length; i++){
    call = functionCalls[i];
    if(call == "temperature"){
      PIDVal = PID_delay();
    }
  }
  
  if(zeroCrossDetected){
    temp.firePulse(PIDVal);
    zeroCrossDetected = false;
  } 
}
