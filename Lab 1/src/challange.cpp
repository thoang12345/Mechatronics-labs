#include <Arduino.h>

const int button_A = A0;
const int button_B = A1;

int button_A_state = 0;
int button_B_state = 0;

const int a_Green = 6;
const int a_Yellow = 7;
const int a_Red = 8;

const int b_Green = 9;
const int b_Yellow = 10;
const int b_Red = 11;

unsigned long last_Change_Time = 0;
int green_Direction = 0;  // 0 = A is green, 1 = B is green
int phase = 0;           // 0 = normal, 1 = yellow, 2 = both red

//these helper functions exist to set the light combonations
void direction_a_green() {
  digitalWrite(a_Green, HIGH);
  digitalWrite(a_Yellow, LOW);
  digitalWrite(a_Red, LOW);

  digitalWrite(b_Green, LOW);
  digitalWrite(b_Yellow, LOW);
  digitalWrite(b_Red, HIGH);
}

void direction_a_yellow() {
  digitalWrite(a_Green, LOW);
  digitalWrite(a_Yellow, HIGH);
  digitalWrite(a_Red, LOW);

  digitalWrite(b_Green, LOW);
  digitalWrite(b_Yellow, LOW);
  digitalWrite(b_Red, HIGH);
}

void direction_b_green() {
  digitalWrite(a_Green, LOW);
  digitalWrite(a_Yellow, LOW);
  digitalWrite(a_Red, HIGH);

  digitalWrite(b_Green, HIGH);
  digitalWrite(b_Yellow, LOW);
  digitalWrite(b_Red, LOW);
}

void direction_b_yellow() {
  digitalWrite(a_Green, LOW);
  digitalWrite(a_Yellow, LOW);
  digitalWrite(a_Red, HIGH);

  digitalWrite(b_Green, LOW);
  digitalWrite(b_Yellow, HIGH);
  digitalWrite(b_Red, LOW);
}

void both_red() {
  digitalWrite(a_Green, LOW);
  digitalWrite(a_Yellow, LOW);
  digitalWrite(a_Red, HIGH);

  digitalWrite(b_Green, LOW);
  digitalWrite(b_Yellow, LOW);
  digitalWrite(b_Red, HIGH);
}

void setup() {
  pinMode(button_A, INPUT);
  pinMode(button_B, INPUT);

  pinMode(a_Green, OUTPUT);
  pinMode(a_Yellow, OUTPUT);
  pinMode(a_Red, OUTPUT);

  pinMode(b_Green, OUTPUT);
  pinMode(b_Yellow, OUTPUT);
  pinMode(b_Red, OUTPUT);

  Serial.begin(9600);  // optional, useful for debugging

  direction_a_green();          // starting condition
  last_Change_Time = millis();  // begin timing from startup
}

void loop() {
  button_A_state = digitalRead(button_A);
  button_B_state = digitalRead(button_B);

  /*Serial.print("A: ");
  Serial.print(button_A_state);
  Serial.print("  B: ");
  Serial.println(button_B_state);*/

  unsigned long current_Time = millis();

  // Normal: one direction is green
  //If a is green only button b can work
  //if b is green only button a can work
  if (phase == 0) {
    bool car_At_Red_Light =
      (green_Direction == 0 && button_B_state == HIGH) || 
      (green_Direction == 1 && button_A_state == HIGH);

    //cool down timers to check if the green light has been 
    //for 10 seconds or if a car has been waiting for at least
    //3 seconds
    bool ten_Seconds_Passed =
      current_Time - last_Change_Time >= 10000;

    bool three_Seconds_Passed =
      current_Time - last_Change_Time >= 3000;

    if (ten_Seconds_Passed || (car_At_Red_Light && three_Seconds_Passed)) {
      if (green_Direction == 0) {
        direction_a_yellow();
      } else {
        direction_b_yellow();
      }

      phase = 1;
      last_Change_Time = current_Time;
    }
  }

  // Yellow has been on for one second
  //it will make the light yellow for one second and then
  //it calls the both_red helper function to make 
  //lights red
  else if (phase == 1) {
    if (current_Time - last_Change_Time >= 1000) {
      both_red();
      phase = 2;
      last_Change_Time = current_Time;
    }
  }

  // Both directions have been red for two seconds
  //this will make sure that both lights are both red
  //for at least 2 seconds. Then it will give the green to the other
  //direction. If a was green, b is now green and vice versa
  else if (phase == 2) {
    if (current_Time - last_Change_Time >= 2000) {
      if (green_Direction == 0) {
        direction_b_green();
        green_Direction = 1;
      } else {
        direction_a_green();
        green_Direction = 0;
      }

      phase = 0;
      last_Change_Time = current_Time;
    }
  }
}