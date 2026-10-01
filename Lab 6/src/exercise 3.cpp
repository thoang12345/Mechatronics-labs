#include <Arduino.h>

const int SW_pin = 2;
const int x_pin = A1;
const int y_pin = A2;

int inputVal = 0;

void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int x_val = analogRead(x_pin);
  int y_val = analogRead(y_pin);
  int but_val = digitalRead(SW_pin);

  if (but_val == LOW) {
    inputVal = 5;  // Joystick button pressed: reset game
  }
  else if (x_val <= 200) {
    inputVal = 1;  // Left
  }
  else if (x_val >= 900) {
    inputVal = 3;  // Right
  }
  else if (y_val <= 200) {
    inputVal = 2;  // Up
  }
  else if (y_val >= 900) {
    inputVal = 4;  // Down
  }
  else {
    inputVal = 0;
  }

  Serial.write(inputVal);
  delay(100);
}