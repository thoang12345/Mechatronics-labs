#include <Arduino.h>
int switchState = 0;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(2);

  if (switchState == LOW){
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  } else {
    digitalWrite(3, LOW);  // green off

    digitalWrite(4, HIGH); // red 1 on
    digitalWrite(5, LOW);  // red 2 off
    delay(250);

    digitalWrite(4, LOW);  // red 1 off
    digitalWrite(5, HIGH); // red 2 on
    delay(250);
  }

  Serial.print("The switchState is:");
  Serial.println(switchState);
}