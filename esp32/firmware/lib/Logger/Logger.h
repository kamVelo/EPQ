#ifndef LOGGER_H
#define LOGGER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <EEPROM.h>
#include "Secrets.h"

#define EEPROM_SIZE 1

class Logger{
    private:
        const char * SSID1 = "SKY06DF3";
        const char *  PWD1 = HOME_WIFI_PASSWORD;

        const char * SSID2 = "Ali Kamel's iPhone";
        const char * PWD2 = HOTSPOT_PASSWORD;

        String dataPassword = DATA_PASSWORD;

        

    public:

    Logger();
    void sendTempData(float temp);
    void sendECData(float EC);


    void failedSendHandler();
    int getNumFailedSends();
    void sendNumFails(int numFails);
    void resetFails();


};


#endif