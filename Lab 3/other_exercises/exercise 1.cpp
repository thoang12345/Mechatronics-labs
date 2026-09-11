#include <Arduino.h>

int photoRes = A1;
int piezo = 8;
int pitch;

void setup() {
  pinMode(photoRes, INPUT);
  pinMode(piezo, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  pitch = map(analogRead(photoRes), 0, 400, 100, 1000);
  Serial.print("The pitch is: ");
  Serial.println(pitch);  
  tone(piezo, pitch, 20);
  delay(10);
}
