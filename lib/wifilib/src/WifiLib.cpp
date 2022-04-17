#include <WiFi.h>
#include <secrets.h>
#include "WifiLib.h"


void connectToWifi() {
  WiFi.mode(WIFI_STA);
  delay(100);
  String hostname = "Esp32-Vivarium";
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

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

void postReadingsToServer(Readings readings) {
  String json = generateJsonBody(readings);
  Serial.println(json);
}