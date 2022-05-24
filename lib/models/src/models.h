#ifndef MODELS_H
#define MODELS_H

struct Reading {
    double humidity;
    double temperature;
    String time;
};

struct Readings {
    Reading bme1;
    Reading bme2;
};

struct TimeHumidityMap {
    int fromHour;
    int humidity;
};

#endif