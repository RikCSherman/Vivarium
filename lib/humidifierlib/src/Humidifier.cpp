#include "Humidifier.h"

#include <Arduino.h>
#include <config.h>
#include <models.h>
#include <pins.h>
#include <queues.h>
#include <utils.h>
#include <wifilib_time.h>

#define HUMIDIFIER_ON true
#define HUMIDIFIER_OFF false

int _pin;
SemaphoreHandle_t mutex;
bool humidifierStatus;

struct TimeHumidityMap humiditySettings[4];

void switchHumidifierOff(bool isHumid) {
    if (humidifierStatus == HUMIDIFIER_ON) {
        digitalWrite(_pin, LOW);
        humidifierStatus = HUMIDIFIER_OFF;
        if (LOGGING_ON) {
            if (isHumid)
                Serial.println("Switching off as humid enough");
        }
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

int getMinHumidity() {
    int retVal = 70;
    int hour = getRawTime().tm_hour;
    for (int i = 0; i < 4; i++) {
        if (hour >= humiditySettings[i].fromHour) {
            retVal = humiditySettings[i].humidity;
        }
    }
    return retVal;
}

bool humidityIsLow(Reading reading) {
    int minHumidity = getMinHumidity();
    bool returnValue = reading.humidity < minHumidity;
    if (LOGGING_ON)
        Serial.printf("lowHumidity = %s : Target %d - Actual %.1f\n", returnValue == true ? "True" : "False",
                      minHumidity, reading.humidity);
    return returnValue;
}

void processReading(Reading reading) {
    bool lowHumidity = humidityIsLow(reading);
    if (lowHumidity) {
        switchHumidifierOn();
    } else {
        switchHumidifierOff(!lowHumidity);
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
    switchHumidifierOff(false);
    humiditySettings[0].fromHour = 0;
    humiditySettings[0].humidity = 80;
    humiditySettings[1].fromHour = 4;
    humiditySettings[1].humidity = 75;
    humiditySettings[2].fromHour = 15;
    humiditySettings[2].humidity = 70;
    humiditySettings[3].fromHour = 22;
    humiditySettings[3].humidity = 75;

    xTaskCreate(receiveReading,                   // Function that should be called
                "Receive Reading in Humidifier",  // Name of the task (for debugging)
                5000,                             // Stack size (bytes)
                NULL,                             // Parameter to pass
                1,                                // Task priority
                NULL                              // Task handle
    );
}