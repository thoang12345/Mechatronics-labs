#include <Arduino.h>
#include <SevSeg.h>

SevSeg sevseg; // Instantiate a seven segment controller object
int pot = A0;
int time;
int coutdown = 1000;
int number = 4999;

void setup() {
  Serial.begin(9600);
  byte numDigits = 4;
  byte digitPins[] = {10,11,12,13};
  byte segmentPins[] = {9,2,3,5,6,8,7,4};
  bool resistorsOnSegments = true; // 'false' means resistors are on digits
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  number = analogRead(pot);
  Serial.println(number);
  sevseg.setNumber(number);
  sevseg.refreshDisplay();
}
