#ifndef UTILS_H
#define UTILS_H

#define LOGGING_ON false

#include <Arduino.h>

String formatTime(tm timeinfo);

const int MAX_ERRORS_ALLOWED = 5;

#endif