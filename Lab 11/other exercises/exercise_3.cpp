#include <Arduino.h>
#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 7;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;

    Serial.println(command);

    IrReceiver.resume();
  }
}