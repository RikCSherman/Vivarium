#include "queues.h"

#include <models.h>

xQueueHandle relayQueue;
xQueueHandle lcdQueue;
xQueueHandle postReadingsQueue;

xQueueHandle waterDistanceQueue;

void initialiseQueues() {
    relayQueue = xQueueCreate(5, sizeof(Readings));
    if (relayQueue == 0) {
        Serial.println("Failed to create relay queue");
    }
    lcdQueue = xQueueCreate(5, sizeof(Readings));
    if (lcdQueue == 0) {
        Serial.println("Failed to create lcd queue");
    }
    postReadingsQueue = xQueueCreate(5, sizeof(Readings));
    if (postReadingsQueue == 0) {
        Serial.println("Failed to create post readings queue");
    }
    waterDistanceQueue = xQueueCreate(5, sizeof(double));
    if (waterDistanceQueue == 0) {
        Serial.println("Failed to create distance queue");
    }
}