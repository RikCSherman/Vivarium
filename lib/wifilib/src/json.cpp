#include "json.h"

double round1(double value) { return (int)(value * 10 + 0.5) / 10.0; }

void addReadingToJson(JsonArray array, int sensor, Reading reading) {
    JsonObject datum = array.createNestedObject();
    datum["sensor"] = sensor;
    datum["temp"] = round1(reading.temperature);
    datum["humidity"] = round1(reading.humidity);
    datum["date_time"] = reading.time;
}

DynamicJsonDocument generateJsonDocument(Readings readings) {
    int size = JSON_ARRAY_SIZE(2) + 3 * JSON_OBJECT_SIZE(4);
    DynamicJsonDocument jsonDoc(size);
    JsonArray data = jsonDoc.createNestedArray("data");
    addReadingToJson(data, 0, readings.bme1);
    addReadingToJson(data, 1, readings.bme2);
    return jsonDoc;
}
