#ifndef QUEUES_H
#define QUEUES_H

#include <Arduino.h>

void initialiseQueues();

extern xQueueHandle humidifierQueue;
extern xQueueHandle lcdQueue;
extern xQueueHandle postReadingsQueue;

extern xQueueHandle waterDistanceQueue;

#endif