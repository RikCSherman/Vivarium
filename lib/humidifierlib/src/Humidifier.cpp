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
const int MIN_HUMIDITY = 70;
const int MIN_TEMPERATURE = 24;
const double MAX_DISTANCE = 18.5;
const int MAX_HUMIDIFIER_ON_TIME = 15 * 60 * 1000 * 1000;
const int MIN_HUMIDIFIER_OFF_TIME = 5 * 60 * 1000 * 1000;

double lastDistance = 99;
Reading lastReading;
int64_t humidifierSwitchedOnTime;
int64_t humidifierSwitchedOffTime;

void switchHumidifierOff() {
    digitalWrite(_pin, LOW);
    humidifierStatus = HUMIDIFIER_OFF;
    humidifierSwitchedOffTime = esp_timer_get_time();
    if (LOGGING_ON)
        Serial.println("Switching off");
}

void switchHumidifierOn() {
    digitalWrite(_pin, HIGH);
    humidifierStatus = HUMIDIFIER_ON;
    humidifierSwitchedOnTime = esp_timer_get_time();
    if (LOGGING_ON)
        Serial.println("Switching on");
}

bool offLongEnough() {
    int offTime = esp_timer_get_time() - humidifierSwitchedOffTime;
    bool returnValue = offTime > MIN_HUMIDIFIER_OFF_TIME;
    if (LOGGING_ON)
        Serial.printf("offLongEnough = %s\n", returnValue == true ? "True" : "False");
    return returnValue;
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

bool enoughWaterInResevoir() {
    bool returnValue = lastDistance < MAX_DISTANCE;
    if (LOGGING_ON)
        Serial.printf("enoughWaterInResevoir = %s\n", returnValue == true ? "True" : "False");
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
        return offLongEnough() && lowHumidity(reading) && temperatureNotLow(reading) && enoughWaterInResevoir();
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

void processDistance(double distanceCM) {
    lastDistance = distanceCM;
    if (humidifierStatus == HUMIDIFIER_ON && !enoughWaterInResevoir()) {
        if (LOGGING_ON)
            Serial.println("Switching off due to distance");
        switchHumidifierOff();
    }
}

void receiveWaterDistance(void *argument) {
    double received;
    while (true) {
        if (xQueueReceive(waterDistanceQueue, &received, portMAX_DELAY) != pdTRUE) {
            Serial.println("Error in Receiving from Distance Queue");
        } else {
            xSemaphoreTake(mutex, portMAX_DELAY);
            processDistance(received);
            xSemaphoreGive(mutex);
        }
        taskYIELD();
    }
}

const uint32_t dontRunTooLongTickDelay = pdMS_TO_TICKS(30 * 1000);
// If humidifier has been on too long switch it off
void dontRunTooLong(void *argument) {
    while (true) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        if (humidifierStatus == HUMIDIFIER_ON &&
            (esp_timer_get_time() - humidifierSwitchedOnTime) > MAX_HUMIDIFIER_ON_TIME) {
            if (LOGGING_ON)
                Serial.println("Switching off due to time");
            switchHumidifierOff();
        }
        xSemaphoreGive(mutex);
        vTaskDelay(dontRunTooLongTickDelay);
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
    xTaskCreate(receiveWaterDistance,              // Function that should be called
                "Receive Distance in Humidifier",  // Name of the task (for debugging)
                5000,                              // Stack size (bytes)
                NULL,                              // Parameter to pass
                1,                                 // Task priority
                NULL                               // Task handle
    );
    xTaskCreate(receiveReading,                   // Function that should be called
                "Receive Reading in Humidifier",  // Name of the task (for debugging)
                5000,                             // Stack size (bytes)
                NULL,                             // Parameter to pass
                1,                                // Task priority
                NULL                              // Task handle
    );
    xTaskCreate(dontRunTooLong,               // Function that should be called
                "max runtime in Humidifier",  // Name of the task (for debugging)
                1000,                         // Stack size (bytes)
                NULL,                         // Parameter to pass
                1,                            // Task priority
                NULL                          // Task handle
    );
}