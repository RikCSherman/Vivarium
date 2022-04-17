#ifndef LCDLIB_H
#define LCDLIB_H

#include <LiquidCrystal_I2C.h>
#include <map>
#include <iterator>
#include <models.h>

void initialiseLCD();
void printReadingsToLCD(Readings readings);

#endif