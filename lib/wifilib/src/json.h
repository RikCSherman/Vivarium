#ifndef JSON_WIFI_H
#define JSON_WIFI_H

#include <ArduinoJson.h>
#include <models.h>

DynamicJsonDocument generateJsonDocument(Readings readings);

#endif