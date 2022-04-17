#ifndef DHTLIB_H
#define DHTLIB_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <map>
#include <models.h>


void initialiseDHTs();
Readings readSensors();

#endif