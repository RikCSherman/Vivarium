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
  std::map<int, readings> readingMap = readSensors();
  printReadingsToLCD(readingMap);
  postReadingsToServer(readingMap);
  std::map<int, readings>::iterator it = readingMap.find(0);
  if (it != readingMap.end()) {
    relaylib.manageRelay(it->second);
  }
}