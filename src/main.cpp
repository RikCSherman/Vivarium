#include <DhtLib.h>
#include <LcdLib.h>
#include <RelayLib.h>
#include <WifiLib.h>
#include <distance.h>
#include <pins.h>
#include <queues.h>

void setup() {
    Serial.begin(115200);
    initialiseQueues();
    initialiseRelay(HUMIDITY_RELAY_PIN);
    initialiseLCD();
    intitialiseWifi();
    initialiseDistanceSensor();
    initialiseDHTs();
}

void loop() { vTaskDelete(NULL); }