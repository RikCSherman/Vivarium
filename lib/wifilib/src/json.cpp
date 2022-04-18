#include <Arduino.h>
#include "json.h"
#include "wifilib_time.h"
#include <utils.h>

String generateReadingJson(int sensor_num, Reading reading) {
  String json_reading = stringFormat("{ sensor: %d, temp: %.1f, humidity: %.1f, date_time: \"", sensor_num, reading.temperature, reading.humidity) + getTime().c_str() + "\"}";
  return json_reading;
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
