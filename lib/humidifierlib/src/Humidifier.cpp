#include "Humidifier.h"

#include <Arduino.h>
#include <config.h>
#include <models.h>
#include <pins.h>
#include <queues.h>
#include <utils.h>

#define HUMIDIFIER_ON true
#define HUMIDIFIER_OFF false

int _pin;
SemaphoreHandle_t mutex;
bool humidifierStatus;

void switchHumidifierOff() {
    if (humidifierStatus == HUMIDIFIER_ON) {
        digitalWrite(_pin, LOW);
        humidifierStatus = HUMIDIFIER_OFF;
        if (LOGGING_ON)
            Serial.println("Switching off");
    }
}

void switchHumidifierOn() {
    if (humidifierStatus == HUMIDIFIER_OFF) {
        digitalWrite(_pin, HIGH);
        humidifierStatus = HUMIDIFIER_ON;
        if (LOGGING_ON)
            Serial.println("Switching on");
    }
}

bool humidityIsLow(Reading reading) {
    bool returnValue = reading.humidity < MIN_HUMIDITY;
    if (LOGGING_ON)
        Serial.printf("lowHumidity = %s\n", returnValue == true ? "True" : "False");
    return returnValue;
}

bool temperatureIsNotTooLow(Reading reading) {
    bool returnValue = reading.temperature > MIN_TEMPERATURE;
    if (LOGGING_ON)
        Serial.printf("temperatureNotLow = %s\n", returnValue == true ? "True" : "False");
    return returnValue;
}

void processReading(Reading reading) {
    if (reading.isError) {
        if (reading.error_count > MAX_ERRORS_ALLOWED) {
            switchHumidifierOff();
        }
    } else if (humidityIsLow(reading) && temperatureIsNotTooLow(reading)) {
        switchHumidifierOn();
    }
}

void receiveReading(void *argument) {
    Reading received;
    while (true) {
        if (xQueueReceive(humidifierQueue, &received, portMAX_DELAY) != pdTRUE) {
            Serial.println("Error in Receiving from Humidifier Queue");
        } else {
            xSemaphoreTake(mutex, portMAX_DELAY);
            processReading(received);
            xSemaphoreGive(mutex);
        }
        taskYIELD();
    }
}

void initialiseHumidifier() {
    _pin = HUMIDIFIER_RELAY_PIN;
    pinMode(_pin, OUTPUT);
    mutex = xSemaphoreCreateMutex();
    switchHumidifierOff();
    xTaskCreate(receiveReading,                   // Function that should be called
                "Receive Reading in Humidifier",  // Name of the task (for debugging)
                5000,                             // Stack size (bytes)
                NULL,                             // Parameter to pass
                1,                                // Task priority
                NULL                              // Task handle
    );
}