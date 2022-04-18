#include <DhtLib.h>
#include <LcdLib.h>
#include <RelayLib.h>
#include <WifiLib.h>
#include <queues.h>

void setup() {
    Serial.begin(115200);
    initialiseQueues();
    initialiseRelay(33);
    initialiseLCD();
    intitialiseWifi();
    initialiseDHTs();
}

void loop() { vTaskDelete(NULL); }