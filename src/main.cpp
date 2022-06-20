#include <Humidifier.h>
#include <LcdLib.h>
#include <WifiLib.h>
#include <bme.h>
#include <pins.h>
#include <queues.h>

void setup() {
    Serial.begin(115200);
    intitialiseWifi();
    initialiseQueues();
    initialiseHumidifier();
    initialiseLCD();
    initialiseBmes();
}

void loop() { vTaskDelete(NULL); }