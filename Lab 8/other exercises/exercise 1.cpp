#include <Arduino.h>
#include <Servo.h>

Servo myServo;
int servo_position = 0; // Variable to store the servo position
int servo_pin = 9; // Pin to which the servo is connected
int pot_pin = A0; // Pin to which the potentiometer is connected

void setup() {
  // put your setup code here, to run once:
  myServo.attach(servo_pin); // Attach servo to pin 9
  Serial.begin(9600); // Start serial communication at 9600 baud
}

void loop() {
  servo_position = analogRead(pot_pin); // Read the potentiometer value
  servo_position = map(servo_position, 0, 1023, 0, 180); // Map the value to servo angle
  myServo.write(servo_position); // Set the servo position
  Serial.print("Angle Value: ");
  Serial.println(servo_position); // Print the angle value to the serial monitor
  delay(15); // Wait for the servo to reach the position
}

