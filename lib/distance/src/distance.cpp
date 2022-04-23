#include <Arduino.h>
#include <pins.h>
#include <queues.h>

const uint32_t readEveryTickDelay = pdMS_TO_TICKS(60 * 1000);

void readDistanceCM(void *argument) {
    while (true) {
        digitalWrite(DISTANCE_TRIGGER_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(DISTANCE_TRIGGER_PIN, LOW);
        unsigned long duration_us = pulseIn(DISTANCE_RESULT_PIN, HIGH);
        double distanceCM = 0.017 * duration_us;
        Serial.printf("Distance %.2fcm\n", distanceCM);
        if (distanceCM > 2.0)
            xQueueSend(waterDistanceQueue, &distanceCM, portMAX_DELAY);
        else if (distanceCM == 0.0)
            vTaskDelay(100);
        else
            vTaskDelay(readEveryTickDelay);
    }
}

void initialiseDistanceSensor() {
    pinMode(DISTANCE_TRIGGER_PIN, OUTPUT);
    pinMode(DISTANCE_RESULT_PIN, INPUT);
    xTaskCreate(readDistanceCM,            // Function that should be called
                "Distance sensor reader",  // Name of the task (for debugging)
                5000,                      // Stack size (bytes)
                NULL,                      // Parameter to pass
                1,                         // Task priority
                NULL                       // Task handle
    );
}