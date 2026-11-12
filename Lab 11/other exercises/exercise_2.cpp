#include <Arduino.h>
#include <Stepper.h>

const int stepsPerRevolution = 2048;
const int change_direction_pin = A1;
const int speed_pin = A0;

bool direction = true;

int lastChangeVal = LOW;
int stableChangeVal = LOW;
unsigned long lastChangeTime = 0;
const unsigned long debounceDelay = 30;

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Button uses an external pull-down resistor
  pinMode(change_direction_pin, INPUT);

  Serial.begin(9600);
}

void loop() {
  int changeVal = digitalRead(change_direction_pin);
  int speed = map(analogRead(speed_pin), 0, 1023, 0, 15);

  // Restart debounce timer whenever the button reading changes
  if (changeVal != lastChangeVal) {
    lastChangeTime = millis();
  }

  // Accept the button state after it stays stable for 30 ms
  if (millis() - lastChangeTime >= debounceDelay) {
    if (changeVal != stableChangeVal) {
      stableChangeVal = changeVal;

      // Toggle direction once per press
      if (stableChangeVal == HIGH) {
        direction = !direction;

        Serial.println(direction
          ? "Turning Clockwise"
          : "Turning Counterclockwise");
      }
    }
  }

  lastChangeVal = changeVal;

  // Only set speed and step when speed is above zero
  if (speed > 0) {
    myStepper.setSpeed(speed);
    myStepper.step(direction ? 1 : -1);
  }
}