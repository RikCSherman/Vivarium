#include "bme.h"

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>
#include <models.h>
#include <queues.h>
#include <wifilib_time.h>

Adafruit_BME280 bme1;
Adafruit_BME280 bme2;

Reading readSensor(Adafruit_BME280 bme) {
    sensors_event_t event;
    struct Reading reading;
    reading.temperature = bme.readTemperature();
    reading.humidity = bme.readHumidity();
    reading.time = getTime();
    return reading;
}

const uint32_t readingTickDelay = pdMS_TO_TICKS(60 * 1000);

void readSensors(void *argument) {
    while (true) {
        Readings both;
        both.bme1 = readSensor(bme1);
        both.bme2 = readSensor(bme2);
        xQueueSend(humidifierQueue, &both.bme1, portMAX_DELAY);
        xQueueSend(lcdQueue, &both, portMAX_DELAY);
        xQueueSend(postReadingsQueue, &both, portMAX_DELAY);
        vTaskDelay(readingTickDelay);
    }
}

void initialiseBmes() {
    bme1.begin(0x76);
    bme2.begin(0x77);
    xTaskCreate(readSensors,          // Function that should be called
                "BME sensor reader",  // Name of the task (for debugging)
                5000,                 // Stack size (bytes)
                NULL,                 // Parameter to pass
                1,                    // Task priority
                NULL                  // Task handle
    );
}