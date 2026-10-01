#include <Arduino.h>

const int pot = A0;  // LED connected to digital pin 3

void setup() {
  pinMode(pot, INPUT);
  Serial.begin(9600);  // Start serial communication
}

void loop() {
    Serial.write(analogRead(pot)/4);  // Read the potentiometer value and send it over serial
    delay(100);  // Wait for 100 milliseconds before the next reading
}