#include "DhtLib.h"
#include <FreeRTOS.h>

#define DHT1PIN 4
#define DHT2PIN 5
#define DHTTYPE DHT22

DHT_Unified dht1(DHT1PIN, DHTTYPE);
DHT_Unified dht2(DHT2PIN, DHTTYPE);

void initialiseDHTs() {
  dht1.begin();
  dht2.begin();
}

Reading readSensor(DHT_Unified dht) {
  sensors_event_t event;
  struct Reading dhtReading;
  dhtReading.isError = false;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    dhtReading.isError = true;
  } else {
    dhtReading.temperature = event.temperature;
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    dhtReading.isError = true;
  } else {
    dhtReading.humidity = event.relative_humidity;
  }
  return dhtReading;
}

Readings readSensors() {
  Readings both;
  both.dht1 = readSensor(dht1);
  both.dht2 = readSensor(dht2);
  return both;
}
