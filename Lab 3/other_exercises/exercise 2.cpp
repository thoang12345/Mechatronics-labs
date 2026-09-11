#include <Arduino.h>

int notes[] = {262, 294, 330, 349};
int button = A0;
int piezo = 8;
int pitch;

void setup() {
  pinMode(button, INPUT);
  pinMode(piezo, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int keyVal = analogRead(button);
  Serial.println(keyVal);
  if (keyVal >= 1015) {
  tone(piezo, notes[0]);

  } else if (keyVal >= 900 && keyVal <= 1010) {
    tone(piezo, notes[1]);

  } else if (keyVal >= 495 && keyVal <= 525) {
    tone(piezo, notes[2]);

  } else if (keyVal >= 1 && keyVal <= 15) {
    tone(piezo, notes[3]);

  } else {
    noTone(piezo);
  }
}
