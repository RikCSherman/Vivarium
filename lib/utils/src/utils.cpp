#include "utils.h"

String formatTime(tm timeinfo) {
    const int dateStringSize = 21;
    char buffer[dateStringSize];
    strftime(buffer, dateStringSize, "%FT%TZ", &timeinfo);
    return String(buffer);
}