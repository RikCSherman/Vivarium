#include "RelayLib.h"

#include <Arduino.h>
#include <queues.h>
#include <utils.h>

int _pin;
String relayStatus;
const int MIN_HUMIDITY = 50;
const int MAX_HUMIDITY = 90;
const double MAX_DISTANCE = 17.5;
double lastDistance = 99;
Reading lastReading;

void switchRelayOff() {
    digitalWrite(_pin, LOW);
    relayStatus = "OFF";
}

void switchRelayOn() {
    digitalWrite(_pin, HIGH);
    relayStatus = "ON";
}

String getRelayStatus() { return relayStatus; }

boolean shouldSwitchRelayOn(Reading reading) {
    if (reading.isError) {
        if (reading.error_count < MAX_ERRORS_ALLOWED)
            return shouldSwitchRelayOn(lastReading);
        else
            return false;
    } else {
        lastReading = reading;
        return reading.humidity < MIN_HUMIDITY && MAX_HUMIDITY < reading.humidity && lastDistance > MAX_DISTANCE;
    }
}

void processReading(Reading reading) {
    if (!reading.isError && reading.humidity < MIN_HUMIDITY) {
        switchRelayOn();
    } else {
        switchRelayOff();
    }
}

void receive_Reading_Relay(void *argument) {
    Reading received;
    while (true) {
        if (xQueueReceive(relayQueue, &received, portMAX_DELAY) != pdTRUE) {
            Serial.println("Error in Receiving from Relay Queue");
        } else {
            processReading(received);
        }
        taskYIELD();
    }
}

void processDistance(double distanceCM) {
    Serial.printf("Distance = %.2fcm\n", distanceCM);
    lastDistance = distanceCM;
    if (lastDistance > MAX_DISTANCE)
        switchRelayOff();
}

void receive_Distance_Relay(void *argument) {
    double received;
    while (true) {
        if (xQueueReceive(waterDistanceQueue, &received, portMAX_DELAY) != pdTRUE) {
            Serial.println("Error in Receiving from Distance Queue");
        } else {
            processDistance(received);
        }
        taskYIELD();
    }
}

void initialiseRelay(int pin) {
    pinMode(pin, OUTPUT);
    _pin = pin;
    switchRelayOff();
    lastReading.error_count = 99;
    lastReading.isError = true;
    xTaskCreate(receive_Distance_Relay,       // Function that should be called
                "Receive Distance in Relay",  // Name of the task (for debugging)
                5000,                         // Stack size (bytes)
                NULL,                         // Parameter to pass
                1,                            // Task priority
                NULL                          // Task handle
    );
    xTaskCreate(receive_Reading_Relay,       // Function that should be called
                "Receive Reading in Relay",  // Name of the task (for debugging)
                5000,                        // Stack size (bytes)
                NULL,                        // Parameter to pass
                1,                           // Task priority
                NULL                         // Task handle
    );
}