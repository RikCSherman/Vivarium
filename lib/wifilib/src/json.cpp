#include <Arduino.h>
#include "json.h"

String generateReadingJson(int sensor_num, Reading reading) {
  return "{ sensor: " + String(sensor_num) + ", temp: " + String(reading.temperature) + ", humidity: " + String(reading.humidity) + "}";
}

String generateJsonBody(Readings readings) {
  String output = "{ data: [";
  if (! readings.dht1.isError) {
    output += generateReadingJson(0, readings.dht1);
  }
  if (! readings.dht1.isError && ! readings.dht2.isError) {
    output += ", ";
  }
  if (! readings.dht2.isError) {
    output += generateReadingJson(1, readings.dht2);
  }
  return output + "]}";
}
