#ifndef MODELS_H
#define MODELS_H


struct Reading {
    float humidity;
    float temperature;
    String time;
    bool isError;
};

struct Readings {
    Reading dht1;
    Reading dht2;
};

#endif