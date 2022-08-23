#include "Temperature.h"

Temperature::Temperature(int tempPin_, float setpoint_, int firePin_){
    tempPin = tempPin_;
    setpoint = setpoint_;
    powerPin = firePin_;

    oneWire = OneWire(tempPin);
    sensors = DallasTemperature(&oneWire);
    sensors.begin();
    pinMode(powerPin, OUTPUT);
}

float Temperature::getTemp(){
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}

void Temperature::heat(float temp){
    if(temp < setpoint - 1.5){
        turnOn();
        delay(15000);
        turnOff();
    }
}

void Temperature::turnOn(){
    digitalWrite(powerPin, HIGH);
}
void Temperature::turnOff(){
    digitalWrite(powerPin, LOW);
}

