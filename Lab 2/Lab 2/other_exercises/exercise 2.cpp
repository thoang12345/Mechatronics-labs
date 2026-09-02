#include <Arduino.h>

const int photoTrans = A1;
const int red = 9;
const int green = 10;
const int blue = 11;
int photoValue;

void setup() {
  pinMode(photoTrans, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  photoValue = map(analogRead(photoTrans), 0, 400, 0, 255);
  Serial.println(photoValue);

  if (photoValue <100) {
    analogWrite(red, photoValue * 2);
    analogWrite(blue, 0);
    analogWrite(green, photoValue * 2);
  } else if (photoValue >=100 && photoValue <197) {
    analogWrite(red, photoValue);
    analogWrite(blue, 0);
    analogWrite(green, photoValue / 2);
  } else if (photoValue >=197) {
    analogWrite(red, photoValue);
    analogWrite(blue, 0);
    analogWrite(green, 0);
  }
}

