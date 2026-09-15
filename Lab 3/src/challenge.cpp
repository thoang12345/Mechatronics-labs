#include <Arduino.h>

int pir = 2;
int led = 13; 
int notes[] = {262, 294, 330, 349};
int button = A0;
int piezo = 8;
int pitch;
bool alarmActive = false;

void setup() {
    pinMode(pir, INPUT);
    pinMode(led, OUTPUT);
    pinMode(button, INPUT);
    pinMode(piezo, OUTPUT);

    Serial.begin(9600);
}

void loop() {
  int pirVal = digitalRead(pir);
    if (pirVal == HIGH) {
        digitalWrite(led, HIGH);
        Serial.println("Motion detected!");
        alarmActive = true;
    } else {
        digitalWrite(led, LOW);
        Serial.println("No motion detected.");
    }

    if (alarmActive) {
        Serial.println("Alarm activated!");
        tone(piezo, notes[0]);
        digitalWrite(led, HIGH);
        delay(10);
        digitalWrite(led, LOW);
    }

    if (analogRead(button) >= 1010) {
        //deactivates the alarm
        alarmActive = false;
        noTone(piezo);
        digitalWrite(led, LOW);
        Serial.println("Alarm deactivated.");
    }

    delay(100);
}