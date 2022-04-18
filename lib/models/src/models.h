#ifndef MODELS_H
#define MODELS_H

struct Reading {
    double humidity;
    double temperature;
    String time;
    bool isError;
};

struct Readings {
    Reading dht1;
    Reading dht2;
};

#endif