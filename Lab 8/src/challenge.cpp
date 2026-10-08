#include <Arduino.h>

const int control_pin_1 = 2;
const int control_pin_2 = 3;
const int on_off_pin = 10;

const int change_direction_pin = A2;
const int start_stop_pin = A1;
const int speed_pin = A0;

int mode = 0;               // 0 = stopped, 1 = running
bool direction = true;

int lastStartVal = LOW;
int lastChangeVal = LOW;

void setup() {
  pinMode(control_pin_1, OUTPUT);
  pinMode(control_pin_2, OUTPUT);
  pinMode(on_off_pin, OUTPUT);

  pinMode(change_direction_pin, INPUT);
  pinMode(start_stop_pin, INPUT);

  digitalWrite(control_pin_1, HIGH);
  digitalWrite(control_pin_2, LOW);
}

void loop() {
  int startVal = digitalRead(start_stop_pin);
  int changeVal = digitalRead(change_direction_pin);
  int speed = map(analogRead(speed_pin), 0, 1023, 0, 255);

  // Toggle running/stopped only when button is newly pressed
  if (startVal == HIGH && lastStartVal == LOW) {
    mode = !mode;
    delay(30);  // basic debounce
  }

  // Change direction only once per press, while running
  if (mode == 1 && changeVal == HIGH && lastChangeVal == LOW) {
    direction = !direction;

    digitalWrite(control_pin_1, direction ? HIGH : LOW);
    digitalWrite(control_pin_2, direction ? LOW : HIGH);

    delay(30);
  }

  // Actually stop motor when mode is 0
  if (mode == 1) {
    analogWrite(on_off_pin, speed);
  } else {
    analogWrite(on_off_pin, 0);
  }

  lastStartVal = startVal;
  lastChangeVal = changeVal;
}