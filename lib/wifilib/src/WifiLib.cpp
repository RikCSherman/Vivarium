#include "WifiLib.h"
#include <WiFi.h>
#include <secrets.h>
#include <models.h>
#include <queues.h>
#include "json.h"

void postReadingsToServer(Readings readings) {
  String json = generateJsonBody(readings);
  Serial.println(json);
}

void receive_Reading_Wifi (void *argument)
{
	Readings received;
	uint32_t TickDelay = pdMS_TO_TICKS(100);
	while (true)
	{
		if (xQueueReceive(postReadingsQueue, &received, portMAX_DELAY) != pdTRUE) {
			Serial.println("Error in Receiving from  wifi Queue");
		} else {
      postReadingsToServer(received);
		}
		vTaskDelay(TickDelay);
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
  xTaskCreate(
    receive_Reading_Wifi,    // Function that should be called
    "Receive Reading to WIFI",   // Name of the task (for debugging)
    50000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
}