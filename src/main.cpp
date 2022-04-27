#include <DhtLib.h>
#include <Humidifier.h>
#include <LcdLib.h>
#include <WifiLib.h>
#include <pins.h>
#include <queues.h>

void setup() {
    Serial.begin(115200);
    intitialiseWifi();
    initialiseQueues();
    initialiseHumidifier();
    initialiseLCD();
    initialiseDHTs();
}

void loop() { vTaskDelete(NULL); }