#include "utils.h"

String formatTime(tm timeinfo) {
    char buffer[21];
    int sizeOfString =
        snprintf(buffer, 100, "%4d-%02d-%02dT%02d:%02d:%02dZ",
                 1900 + timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday,
                 timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    if (sizeOfString <= 0 || sizeOfString > 21) {
        Serial.println("Oops problem with time printing");
        return "1970-00-01T00:00:01Z";
    }
    return String(buffer);
}