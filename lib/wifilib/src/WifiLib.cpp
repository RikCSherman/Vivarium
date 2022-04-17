#include <WiFi.h>
#include <secrets.h>
#include "WifiLib.h"
#include <iterator>


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

String generateJsonBody(std::map<int, readings> readingMap) {
  String output = "{ data: [";
    std::map<int, readings>::iterator it = readingMap.begin();
    while(it != readingMap.end())
    {
      if(it->second.isError) {
        it++;
      } else {
        output = output + "{ sensor: " + it->first + ", temp: " + it->second.temperature + ", humidity: " + it->second.humidity + "},";
        it++;
      }
    }
  output = output.substring(0, output.length() - 1);
  return output + "]}";
}

void postReadingsToServer(std::map<int, readings> readingMap) {
  String json = generateJsonBody(readingMap);
}