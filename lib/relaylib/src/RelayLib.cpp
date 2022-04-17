#include "RelayLib.h"
#include <Arduino.h>

void RelayLib::switchRelayOff() {
  digitalWrite(_pin, LOW);
  _relayStatus = "OFF";
}

void RelayLib::switchRelayOn() {
  digitalWrite(_pin, HIGH);
  _relayStatus = "ON";
}

RelayLib::RelayLib(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
  switchRelayOff();
}

String RelayLib::getRelayStatus() {
    return _relayStatus;
}

void RelayLib::manageRelay(readings reading) {
    if (!reading.isError && reading.humidity < 50) {
      switchRelayOn();
    } else {
      switchRelayOff();
    }
}