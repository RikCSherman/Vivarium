#ifndef MODELS_H
#define MODELS_H


struct Reading {
    float humidity;
    float temperature;
    bool isError;
};

struct Readings {
    Reading dht1;
    Reading dht2;
};

const int minHumidity = 75;
const int maxHumidity = 90;

#endif