#include <Arduino.h>
#include <esp_sntp.h>
#include <utils.h>

void initialiseTime() { configTime(0, 0, "pool.ntp.org"); }

const uint32_t dhtReadingTickDelay = pdMS_TO_TICKS(10);

tm getRawTime() {
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo, 50)) {
        vTaskDelay(dhtReadingTickDelay);
    }
    return timeinfo;
}

String getTime() { return formatTime(getRawTime()); }