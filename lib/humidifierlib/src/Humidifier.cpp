#include "Humidifier.h"

#include <Arduino.h>
#include <models.h>
#include <pins.h>
#include <queues.h>
#include <utils.h>

#define HUMIDIFIER_ON true
#define HUMIDIFIER_OFF false

int _pin;
SemaphoreHandle_t mutex;
bool humidifierStatus;
const int MIN_HUMIDITY = 75;
const int MIN_TEMPERATURE = 24;

Reading lastReading;
int64_t humidifierSwitchedOnTime;
int64_t humidifierSwitchedOffTime;

void switchHumidifierOff() {
    if (humidifierStatus == HUMIDIFIER_ON) {
        digitalWrite(_pin, LOW);
        humidifierStatus = HUMIDIFIER_OFF;
        humidifierSwitchedOffTime = esp_timer_get_time();
        if (LOGGING_ON)
            Serial.println("Switching off");
    }
}

void switchHumidifierOn() {
    if (humidifierStatus == HUMIDIFIER_OFF) {
        digitalWrite(_pin, HIGH);
        humidifierStatus = HUMIDIFIER_ON;
        humidifierSwitchedOnTime = esp_timer_get_time();
        if (LOGGING_ON)
            Serial.println("Switching on");
    }
}

bool lowHumidity(Reading reading) {
    bool returnValue = reading.humidity < MIN_HUMIDITY;
    if (LOGGING_ON)
        Serial.printf("lowHumidity = %s\n", returnValue == true ? "True" : "False");
    return returnValue;
}

bool temperatureNotLow(Reading reading) {
    bool returnValue = reading.temperature > MIN_TEMPERATURE;
    if (LOGGING_ON)
        Serial.printf("temperatureNotLow = %s\n", returnValue == true ? "True" : "False");
    return returnValue;
}

bool shouldSwitchHumidifierOn(Reading reading) {
    if (reading.isError) {
        if (reading.error_count < MAX_ERRORS_ALLOWED)
            return shouldSwitchHumidifierOn(lastReading);
        else
            return false;
    } else {
        lastReading = reading;
        return lowHumidity(reading) && temperatureNotLow(reading);  //&& enoughWaterInResevoir();
    }
}

void processReading(Reading reading) {
    if (shouldSwitchHumidifierOn(reading)) {
        switchHumidifierOn();
    } else {
        switchHumidifierOff();
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
    humidifierSwitchedOffTime = 0;  // so that it can turn on within 10 minutes of startup
    lastReading.error_count = 99;
    lastReading.isError = true;
    xTaskCreate(receiveReading,                   // Function that should be called
                "Receive Reading in Humidifier",  // Name of the task (for debugging)
                5000,                             // Stack size (bytes)
                NULL,                             // Parameter to pass
                1,                                // Task priority
                NULL                              // Task handle
    );
}