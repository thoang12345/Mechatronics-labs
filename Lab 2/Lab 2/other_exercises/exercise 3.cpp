#include <Arduino.h>

const int echo = 6;
const int trig = 5;

void setup() {
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT); 
  
  Serial.begin(9600);
}

void loop() {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH);

    float distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(500);
 
}

