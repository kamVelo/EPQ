#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
class Temperature{
    private:
        int tempPin,setpoint, firePin;
        int previousReadTime = 0;
        int maximumFiringDelay = 7400;

        // PID variables
        float PIDError = 0;
        float prevError = 0;
        float elapsedTime,Time,timePrev;

        // PID parameters
        float kp = 203;
        float ki = 7.2;
        float kd = 1.04;
        float PID_p = 0;
        float PID_i = 0; 
        float PID_d = 0;

        // misc variables
        OneWire oneWire;
        DallasTemperature sensors;

    public:
        void begin(int tempPin_, float setpoint_, int firePin_);
        float getTemp();
        float getPIDDelay();
        void fireHeaterPulse(float PIDValue);

};

#endif