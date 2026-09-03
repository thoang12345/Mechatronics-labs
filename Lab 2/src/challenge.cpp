#include <Arduino.h>

const int echo = 6;
const int trig = 5;

const int photoTrans = A1;
const int red = 9;
const int green = 10;
const int blue = 11;
int photoValue;

void setup() {
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT); 
  pinMode(photoTrans, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
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
    float feet = distance / 30.48;

    Serial.print("Distance: ");
    Serial.print(feet);
    Serial.println(" ft");

    

    if (feet <= 1) {
    Serial.println("Danger, you're doomed.");
    analogWrite(red, 255);
    analogWrite(blue, 0);
    analogWrite(green, 0);
  } else if (feet <= 3 && feet > 1) {
    analogWrite(red, 255);
    analogWrite(blue, 0);
    analogWrite(green, 255);
  } else if (feet > 3) {
    analogWrite(red, 0);
    analogWrite(blue, 0);
    analogWrite(green, 255);
  }

    delay(400);
}

