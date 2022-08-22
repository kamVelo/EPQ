#include "Temperature.h"

void Temperature::begin(int tempPin_, float setpoint_, int firePin_)
{
    tempPin = tempPin_;
    setpoint = setpoint_;
    firePin = firePin_;

    OneWire oneWire(tempPin);
    DallasTemperature sensors(&oneWire);
    sensors.begin();
    pinMode(firePin, OUTPUT);
    digitalWrite(firePin, LOW);
}

float Temperature::getTemp(){
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}

float getPIDDelay(float temp){
    return 0;
}
void Temperature::fireHeaterPulse(float PIDValue){
    //delayMicroseconds(maximumFiringDelay - PIDValue);
    //delay(15);
    //digitalWrite(firePin, HIGH);
    //delayMicroseconds(100);
    digitalWrite(firePin, LOW);
}