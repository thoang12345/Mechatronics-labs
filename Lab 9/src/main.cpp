#include <Arduino.h>

const int red_led = 3;
const int green_led = 2;

const int pot1 = A0;
const int pot2 = A1;

int pot_1 = 0;
int pot_2 = 0;
float voltage_1 = 0;
float voltage_2 = 0;

void setup() {
  Serial.begin(9600);

  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);
}

void loop() {
  pot_1 = analogRead(pot1);
  pot_2 = analogRead(pot2);

  voltage_1 = pot_1 * (5.0 / 1023.0);
  voltage_2 = pot_2 * (5.0 / 1023.0);

  Serial.print("Raw 1: ");
  Serial.print(pot_1);
  Serial.print(", Raw 2: ");
  Serial.println(pot_2);

  Serial.print("Voltage 1: ");
  Serial.print(voltage_1, 3);
  Serial.print(" V, Voltage 2: ");
  Serial.print(voltage_2, 3);
  Serial.print(" V | ");

  // voltage_1: x-axis; voltage_2: y-axis
  if (voltage_2 > 1.6027 * voltage_1 - 1.4846) {
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);
    Serial.println("Red LED");
  } else {
    digitalWrite(green_led, HIGH);
    digitalWrite(red_led, LOW);
    Serial.println("Green LED");
  }

  delay(100);
}