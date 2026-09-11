#include <Arduino.h>

int pir = 2;
int led = 13; 

void setup() {
    pinMode(pir, INPUT);
    pinMode(led, OUTPUT);

    Serial.begin(9600);
}

void loop() {
    int pirVal = digitalRead(pir);
    if (pirVal == HIGH) {
        digitalWrite(led, HIGH);
        Serial.println("Motion detected!");
    } else {
        digitalWrite(led, LOW);
        Serial.println("No motion detected.");
    }

    delay(100);
}
