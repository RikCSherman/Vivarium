#include <Arduino.h>
#include <esp_sntp.h>
#include <utils.h>

void initialiseTime() { configTime(0, 0, "pool.ntp.org"); }

String getTime() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    return formatTime(timeinfo);
}