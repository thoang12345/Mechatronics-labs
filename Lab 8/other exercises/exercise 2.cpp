#include <Arduino.h>

const int control_pin_1 = 2, control_pin_2 = 3, on_off_pin = 10;

void setup() {
  pinMode(control_pin_1, OUTPUT);
  pinMode(control_pin_2, OUTPUT);
  pinMode(on_off_pin, OUTPUT);
}

void loop() {
  analogWrite(on_off_pin, 250); // Turn on the motor at full speed
    digitalWrite(control_pin_1, HIGH); // Set control pin 1 HIGH
    digitalWrite(control_pin_2, LOW); // Set control pin 2 LOW  
    delay(2000); // Wait for 2 seconds
    digitalWrite(control_pin_1, LOW); // Set control pin 1 LOW
    digitalWrite(control_pin_2, HIGH); // Set control pin 2 HIGH
    delay(2000); // Wait for 2 seconds

}

