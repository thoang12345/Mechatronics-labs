#include <Arduino.h>

const int ledPin = 3;  // LED connected to digital pin 3

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  // Start serial communication
}

void loop() {
  if (Serial.available() > 0) {  // Check for serial data
    int ledPinState = Serial.read();

    if (ledPinState == '1') {
      digitalWrite(ledPin, HIGH);
    }

    if (ledPinState == '0') {
      digitalWrite(ledPin, LOW);
    }
  }
}