#include "WifiLib.h"

#include <HTTPClient.h>
#include <WiFi.h>
#include <models.h>
#include <queues.h>
#include <secrets.h>
#include <utils.h>

#include "json.h"
#include "wifilib_time.h"

const String url = host + "/api/measurements";

void postReadingsToServer(Readings readings) {
    DynamicJsonDocument jsonDoc = generateJsonDocument(readings);
    char jsonData[1024];
    serializeJson(jsonDoc, jsonData);
    if (LOGGING_ON) {
        Serial.println(jsonData);
    }
    WiFiClient client;
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/json", "Content-Length", String(jsonData).length());
    http.setAuthorization(apiKey);
    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode != 201)
        Serial.printf("Upload of data failed code = %d\n", httpResponseCode);
}

void receive_Reading_Wifi(void* argument) {
    Readings readings;
    while (true) {
        if (xQueueReceive(postReadingsQueue, &readings, portMAX_DELAY) != pdTRUE) {
            Serial.println("Error in Receiving from  wifi Queue");
        } else {
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
    getTime();

    xTaskCreate(receive_Reading_Wifi,       // Function that should be called
                "Receive Reading to WIFI",  // Name of the task (for debugging)
                5000,                       // Stack size (bytes)
                NULL,                       // Parameter to pass
                1,                          // Task priority
                NULL                        // Task handle
    );
}