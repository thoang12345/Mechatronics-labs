// Include the Arduino Stepper.h library
#include "Stepper.h"
#include <Arduino.h>

// Define number of steps per rotation (2048 = 360 degrees)
const int stepsPerRevolution = 2048;

// Create stepper object called 'myStepper', note the pin order:
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  // Set speed to 15 RPM
  myStepper.setSpeed(15);

  // Start Serial monitor
  Serial.begin(9600);
}

void loop() {
  // Turn Clockwise
  Serial.println("Turning Clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500); // Wait half a second

  // Turn Counterclockwise
  Serial.println("Turning Counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(500);
}