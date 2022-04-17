#ifndef RELAYLIB_H
#define RELAYLIB_H

#include <models.h>
#include <Arduino.h>

void initialiseRelay();

void manageRelay(readings reading);

class RelayLib {
    public: 
        RelayLib(int pin);
        void manageRelay(readings reading);
        String getRelayStatus();
    private:
        int _pin;
        String _relayStatus;
        void switchRelayOff();
        void switchRelayOn();
};

#endif