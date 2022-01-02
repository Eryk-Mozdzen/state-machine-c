#include "rc5_transmitter.h"

TransmitterRC5::TransmitterRC5(int pin) {
  this->pin = pin;
  
  pinMode(this->pin, OUTPUT);
}

void TransmitterRC5::sendBit(bool bit) {
  if(bit) {
    digitalWrite(this->pin, HIGH);
    delayMicroseconds(PULSE);
    digitalWrite(this->pin, LOW);
    delayMicroseconds(SPACE);
  } else {
    digitalWrite(this->pin, LOW);
    delayMicroseconds(SPACE);
    digitalWrite(this->pin, HIGH);
    delayMicroseconds(PULSE);
  }
}

void TransmitterRC5::send(uint8_t toggle, uint8_t address, uint8_t command) {
  toggle &=0x01;
  address &=0x1F;
  command &=0x3F;
  
  uint16_t frame = (3<<12) | ((uint16_t)toggle<<11) | ((uint16_t)address<<6) | command;

  Serial.println(frame, HEX);

  for(int i=13; i>=0; i--) {
    bool bit = (frame & (1<<i))>0;

    this->sendBit(bit);
    Serial.print(bit);
  }
  Serial.println("");

  digitalWrite(this->pin, HIGH);
}

