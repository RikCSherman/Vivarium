#ifndef MODELS_H
#define MODELS_H


struct readings {
    float humidity;
    float temperature;
    bool isError;
};

const int minHumidity = 75;
const int maxHumidity = 90;

#endif