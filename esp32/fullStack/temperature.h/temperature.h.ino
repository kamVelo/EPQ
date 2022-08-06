#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <OneWire>
#include <DallasTemperature>

class Temperature{
  int tempPin;
  int setpoint;
  int previousReadTime = 0;
  // PID variables:
  float PIDError = 0;
  float prevError = 0;
  float elapsedTime, Time, timePrev;
  // PID Parameters:
  float kp = 203;
  float ki = 7.2;
  float kd = 1.04;
  float PID_p = 0;
  float PID_i = 0; 
  float PID_d = 0;

  
  void temperature(int tempPin_, float setpoint_, int firePin_){
    tempPin = tempPin_;
    setpoint = setpoint_;
    firePin = firePin_;

    
    OneWire oneWire(tempPin);
    DallasTemperature sensors(&oneWire);

    pinMode(firePin, OUTPUT);
  }
  float getTemp(){
    // this function returns current temperature read from ds18b20
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
  }
  float getPIDDelay(){
    /*
     * this function returns the ms delay for pid control
     * of the 220v heater
     */
    currTime = millis();
    elapsedTime = currTime - previousReadTime;
    previousReadTime = currTime;
    
    PIDError = setpoint - getTemp();

    // integral constant only affects errors under 30 C
    if(PID_error > 30) PID_i = 0;

    PID_p = kp * PIDError;
    PID_i = PID_i + (ki * PIDError);
    PID_d = kd*((PIDError-prevError)/elapsedTime);

    // calculate PID value between 0 and 7400us
    PIDValue = PID_p + PID_i + PID_d;
    if(PID_value < 0) PID_value = 0;
    elif(PID_value > 7400) PID_value = 7400;
    
    // storing prevError
    prevError = PIDError;
    
    // returning delay
    return PID_value;
  }

  void firePulse(float PIDValue){
    delayMicroseconds(maximumFiringDelay - PIDValue);
    digitalWrite(firingPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(firingPin, LOW);
  }
}

#endif
