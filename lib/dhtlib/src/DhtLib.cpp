#include "DhtLib.h"

#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <models.h>
#include <pins.h>
#include <queues.h>
#include <utils.h>
#include <wifilib_time.h>

#include <map>

#define DHTTYPE DHT22

DHT_Unified dht1(DHT1PIN, DHTTYPE);
DHT_Unified dht2(DHT2PIN, DHTTYPE);

Reading readSensor(DHT_Unified dht) {
    sensors_event_t event;
    struct Reading dhtReading;
    dhtReading.isError = false;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        dhtReading.isError = true;
    } else {
        dhtReading.temperature = event.temperature;
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        dhtReading.isError = true;
    } else {
        dhtReading.humidity = event.relative_humidity;
    }
    dhtReading.time = getTime();
    return dhtReading;
}

const uint32_t tickDelay = pdMS_TO_TICKS(3 * 1000);

void readSensors(void *argument) {
    while (true) {
        Readings both;
        both.dht1 = readSensor(dht1);
        both.dht2 = readSensor(dht2);
        xQueueSend(relayQueue, &both, portMAX_DELAY);
        xQueueSend(lcdQueue, &both, portMAX_DELAY);
        xQueueSend(postReadingsQueue, &both, portMAX_DELAY);
        vTaskDelay(tickDelay);
    }
}

void initialiseDHTs() {
    dht1.begin();
    dht2.begin();
    xTaskCreate(readSensors,          // Function that should be called
                "DHT sensor reader",  // Name of the task (for debugging)
                50000,                // Stack size (bytes)
                NULL,                 // Parameter to pass
                1,                    // Task priority
                NULL                  // Task handle
    );
}