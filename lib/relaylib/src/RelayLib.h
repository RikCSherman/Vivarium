#ifndef RELAYLIB_H
#define RELAYLIB_H

#include <models.h>
#include <Arduino.h>

class RelayLib {
    public: 
        RelayLib(int pin);
        void manageRelay(Reading reading);
        String getRelayStatus();
    private:
        int _pin;
        String _relayStatus;
        void switchRelayOff();
        void switchRelayOn();
};

#endif