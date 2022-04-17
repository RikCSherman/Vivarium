#include <DhtLib.h>
#include <WifiLib.h>
#include <LcdLib.h>
#include <RelayLib.h>
#include <models.h>
#include <iterator>

uint32_t delayMS = 3000;

RelayLib relaylib(33);

void setup() {
  Serial.begin(115200);
  initialiseDHTs();
  initialiseLCD();
  connectToWifi();
}

void loop() {
  delay(delayMS);
  Readings readings = readSensors();
  printReadingsToLCD(readings);
  postReadingsToServer(readings);
  relaylib.manageRelay(readings.dht1);
}