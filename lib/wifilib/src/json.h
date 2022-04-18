#ifndef JSON_WIFI_H
#define JSON_WIFI_H

#include <Arduino.h>
#include <models.h>

String generateReadingJson(int sensor_num, Reading reading);

String generateJsonBody(Readings readings);

#endif