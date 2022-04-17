#include "RelayLib.h"
#include <Arduino.h>
#include <queues.h>

int _pin;
String relayStatus;
const int minHumidity = 50;
const int maxHumidity = 90;


void switchRelayOff() {
  digitalWrite(_pin, LOW);
  relayStatus = "OFF";
}

void switchRelayOn() {
  digitalWrite(_pin, HIGH);
  relayStatus = "ON";
}

String getRelayStatus() {
    return relayStatus;
}

void manageRelay(Reading reading) {
    if (!reading.isError && reading.humidity < minHumidity) {
      switchRelayOn();
    } else {
      switchRelayOff();
    }
}

void receive_Reading_Relay (void *argument)
{
	Reading received;
	uint32_t TickDelay = pdMS_TO_TICKS(100);
	while (true)
	{
		if (xQueueReceive(relayQueue, &received, portMAX_DELAY) != pdTRUE) {
			Serial.println("Error in Receiving from Queue");
		} else {
      manageRelay(received);
		}
		vTaskDelay(TickDelay);
	}
}

void initialiseRelay(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  switchRelayOff();
  xTaskCreate(
    receive_Reading_Relay,    // Function that should be called
    "Receive Reading to Relay",   // Name of the task (for debugging)
    50000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );
}