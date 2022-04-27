#include "queues.h"

#include <models.h>

xQueueHandle humidifierQueue;
xQueueHandle lcdQueue;
xQueueHandle postReadingsQueue;

void initialiseQueues() {
    humidifierQueue = xQueueCreate(5, sizeof(Readings));
    if (humidifierQueue == 0) {
        Serial.println("Failed to create humidifier queue");
    }
    lcdQueue = xQueueCreate(5, sizeof(Readings));
    if (lcdQueue == 0) {
        Serial.println("Failed to create lcd queue");
    }
    postReadingsQueue = xQueueCreate(5, sizeof(Readings));
    if (postReadingsQueue == 0) {
        Serial.println("Failed to create post readings queue");
    }
}