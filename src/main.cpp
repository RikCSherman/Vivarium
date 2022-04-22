#include <DhtLib.h>
#include <LcdLib.h>
#include <RelayLib.h>
#include <WifiLib.h>
#include <distance.h>
#include <pins.h>
#include <queues.h>

void setup() {
    Serial.begin(115200);
    intitialiseWifi();
    initialiseQueues();
    initialiseHumidifier();
    initialiseLCD();
    initialiseDistanceSensor();
    initialiseDHTs();
}

void loop() { vTaskDelete(NULL); }