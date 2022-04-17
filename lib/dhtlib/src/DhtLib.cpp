#include "DhtLib.h"

#define DHT1PIN 4
#define DHT2PIN 5
#define DHTTYPE DHT22

DHT_Unified dht1(DHT1PIN, DHTTYPE);
DHT_Unified dht2(DHT2PIN, DHTTYPE);

void initialiseDHTs() {
  dht1.begin();
  dht2.begin();
}

readings readSensor(DHT_Unified dht) {
  sensors_event_t event;
  struct readings reading;
  reading.isError = false;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    reading.isError = true;
  } else {
    reading.temperature = event.temperature;
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    reading.isError = true;
  } else {
    reading.humidity = event.relative_humidity;
  }
  return reading;
}

std::map<int, readings> readSensors() {
    readings reading1 = readSensor(dht1);
    readings reading2 = readSensor(dht2);
    std::map<int, readings> mapOfReadings;
    mapOfReadings.insert(std::make_pair(0, reading1));
    mapOfReadings.insert(std::make_pair(1, reading2));
    return mapOfReadings;
}
