#include "WifiLib.h"

#include <HTTPClient.h>
#include <WiFi.h>
#include <models.h>
#include <queues.h>
#include <secrets.h>

#include "json.h"
#include "wifilib_time.h"

const String url = host + "/api/measurements";

void postReadingsToServer(Readings readings) {
    DynamicJsonDocument jsonDoc = generateJsonDocument(readings);
    char jsonData[1024];
    serializeJson(jsonDoc, jsonData);
    Serial.println(jsonData);
    WiFiClient client;
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json", "Content-Length", String(jsonData).length());
    http.setAuthorization(apiKey);
    int httpResponseCode = http.POST(jsonData);
    Serial.printf("Response code %d", httpResponseCode);
}

void receive_Reading_Wifi(void* argument) {
    Readings readings;
    while (true) {
        if (xQueueReceive(postReadingsQueue, &readings, portMAX_DELAY) != pdTRUE) {
            Serial.println("Error in Receiving from  wifi Queue");
        } else {
            if (!(readings.dht1.isError && readings.dht2.isError))
                postReadingsToServer(readings);
        }
        taskYIELD();
    }
}

void intitialiseWifi() {
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
    initialiseTime();
    Serial.println(getTime());

    xTaskCreate(receive_Reading_Wifi,       // Function that should be called
                "Receive Reading to WIFI",  // Name of the task (for debugging)
                5000,                       // Stack size (bytes)
                NULL,                       // Parameter to pass
                1,                          // Task priority
                NULL                        // Task handle
    );
}