#include <DhtLib.h>
#include <LcdLib.h>
#include <RelayLib.h>
#include <WifiLib.h>
#include <pins.h>
#include <queues.h>

void setup() {
    Serial.begin(115200);
    initialiseQueues();
    initialiseRelay(HUMIDITY_RELAY_PIN);
    initialiseLCD();
    intitialiseWifi();
    initialiseDHTs();
}

void loop() { vTaskDelete(NULL); }