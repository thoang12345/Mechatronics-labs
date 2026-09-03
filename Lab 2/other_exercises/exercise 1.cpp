#include <Arduino.h>

const int temp_resist = A0; // Analog pin for temperature sensor
float resist = 0;

void setup() {
  for(int pinNumber = 2; pinNumber <= 13; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
  pinMode(temp_resist, INPUT);
  Serial.begin(9600);
}

void loop() {
  resist = analogRead(temp_resist);

  if(resist<20) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  } else if(resist<25 and resist>=22){
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  } else if(resist<27 and resist>=25){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  } else if(resist<29 and resist>=27){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  }
  Serial.println(resist);

  delay(1);
}

