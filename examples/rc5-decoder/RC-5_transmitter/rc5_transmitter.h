#pragma once

#include <stdint.h>
#include <Arduino.h>

#define SPACE   889 // us
#define PULSE   889 // us

class TransmitterRC5 {
    int pin;

    void sendBit(bool);
  public:
    TransmitterRC5(int);

    void send(uint8_t, uint8_t, uint8_t);
};
