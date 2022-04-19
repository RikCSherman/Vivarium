#include "utils.h"

String formatTime(tm timeinfo) {
    char buffer[21];
    snprintf(buffer, 100, "%4d-%02d-%02dT%02d:%02d:%02dZ",
             1900 + timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday,
             timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    return String(buffer);
}