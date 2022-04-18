#include <Arduino.h>
#include <esp_sntp.h>
#include <utils.h>


void initialiseTime() {
    configTime(0, 0, "pool.ntp.org");
}

String getTime() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  return stringFormat("%4d-%02d-%02dT%02d:%02d:%02dZ", 1900 + timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}