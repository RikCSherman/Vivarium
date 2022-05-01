#ifndef MODELS_H
#define MODELS_H

struct Reading {
    double humidity;
    double temperature;
    String time;
    bool isError;
    int error_count;
};

struct Readings {
    Reading dht1;
    Reading dht2;
};

struct TimeHumidityMap {
    int fromHour;
    int humidity;
};

#endif