#include <Arduino.h>
#include "SevSeg.h"

SevSeg sevseg;

// set constants
const int pot = A0;           // potentiometer
const int startButton = A3;   // stop/start button
const int changeButton = A2;  // when stopped, adjust countdown time
const int piezo = A4;         // buzzer
const int alarmButton = A1;   // alarm cutout

int potValue = 0;

int changeVal = 0;  // High/Low from alarm change button when timing = false
int startVal = 0;   // High/Low reading from stop start button
int lastChange = 0; // extra variable to define state of countdown
int lastStart = 0;  // 0 when low, and 1 when high for both change and start
int lastAlarm = 0;

long beepTime = 0;

long potTime = 0;
long countdownTime = 0;

int minutes = 0;
int seconds = 0;
long previousTime = 0; // track millis

int mode = 0; // mode 0 set time, mode 1 run, mode 2 pause, mode 3 alarm

void setup() {
  Serial.begin(9600);
  pinMode(startButton, INPUT);
  pinMode(changeButton, INPUT);
  pinMode(alarmButton, INPUT);
  pinMode(piezo, OUTPUT);

  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;

  sevseg.begin(
    hardwareConfig,
    numDigits,
    digitPins,
    segmentPins,
    resistorsOnSegments,
    false
  );
  sevseg.setBrightness(100);
}

void loop() {
  startVal = digitalRead(startButton);
  changeVal = digitalRead(changeButton);
  int alarmVal = digitalRead(alarmButton);
  sevseg.refreshDisplay();

  switch (mode) {
    case 0: // time mode

      // if (changeVal == HIGH && lastChange == LOW) { // change button activated
      potTime = map(analogRead(pot), 0, 1023, 0, 300); // 0-5 minutes, this is the map that sets the time
      //if you want the time to be larger, we can increase the 300
      countdownTime = potTime;
      // }

      if (startVal == HIGH && lastStart == LOW && countdownTime > 0) { // when the start button is pressed, the timer will start to go down. 
        previousTime = millis();
        mode = 1;
      }
      break;

    case 1: // running mode
      if (millis() - previousTime >= 1000) { // checks to see if 1 second has passed
        previousTime = previousTime + 1000;

        //once the countdown reaches 0, it switches to mode 3 (alarm mode)
        if (countdownTime > 0) {
          countdownTime = countdownTime - 1;
        } else {
          beepTime = 0;
          mode = 3;
        }
      }

      if (startVal == HIGH && lastStart == LOW) { // pauses the clock if the pause button is pressed 
        mode = 2;
      }
      break;

    case 2: // when paused
      if (startVal == HIGH && lastStart == LOW) { // resume the alarm from where it is 
        previousTime = millis();
        mode = 1;
      }

      if (changeVal == HIGH && lastChange == LOW) { // change time to the original set time
        mode = 0;
      }
      break;

    case 3: // alarming
      if (beepTime == 0) { //beep
        tone(piezo, 2500, 250);
        beepTime = millis();
      }

      if (millis() - beepTime >= 3000) { // after 3 seconds from the start of the first beep, it play another beep.
        // if you want to decrease the beeping interval, you must change this 3000 value to something else.
        tone(piezo, 2500, 250);
        beepTime = millis(); // resets the count
      }

      Serial.println("The alarm is on. Please press the alarm button to turn off.");

      if (alarmVal == HIGH && lastAlarm == LOW) { // if alarm off button pressed
        noTone(piezo);
        countdownTime = 0;
        beepTime = 0;
        mode = 0;
      }
      break;
  }

  lastStart = startVal;
  lastChange = changeVal;
  lastAlarm = alarmVal;

  minutes = countdownTime / 60;
  seconds = countdownTime % 60;

  int number = (minutes * 100) + seconds;
  sevseg.setNumber(number);
}