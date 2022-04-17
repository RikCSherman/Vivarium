#ifndef QUEUES_H
#define QUEUES_H
#include <Arduino.h>

void initialiseQueues();

extern xQueueHandle relayQueue;
extern xQueueHandle lcdQueue;
extern xQueueHandle postReadingsQueue;

#endif