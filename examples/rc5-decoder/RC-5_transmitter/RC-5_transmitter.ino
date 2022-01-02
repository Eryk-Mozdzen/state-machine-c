#include "rc5_transmitter.h"

TransmitterRC5 transmitter(7);

void setup(){
  Serial.begin(115200);

  pinMode(13, OUTPUT);
}

uint8_t toggle = 0;
uint8_t address = 1;
uint8_t command = 0;

void loop(){

  digitalWrite(13, toggle);

  Serial.print("Toggle: ");
  Serial.print(toggle);
  Serial.print(" Address: 0x");
  Serial.print(address, HEX);
  Serial.print(" Command: 0x");
  Serial.println(command, HEX);

  transmitter.send(toggle, address, command);

  toggle = !toggle;
  command++;
  command %=0x3F;
  
  delay(1000);
}
