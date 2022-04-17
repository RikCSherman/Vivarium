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

#endif