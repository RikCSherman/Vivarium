#ifndef UTILS_H
#define UTILS_H

#define LOGGING_ON true

#include <Arduino.h>
#include <time.h>

String formatTime(tm timeinfo);

const int MAX_ERRORS_ALLOWED = 5;

#endif