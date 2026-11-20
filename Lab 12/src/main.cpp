/*  
Arduino connections:
    A0 -> joystick VRx                 A1 -> joystick VRy
    D2 -> HC-SR04 TRIG                 D3 <- HC-SR04 ECHO
    D4 -> left LED through resistor    D12 -> right LED through resistor
    D5 -> L293D pin 1 (enable 1,2)      D6 -> passive piezo +
    D7 -> L293D pin 2 (input 1)         D8 -> L293D pin 7 (input 2)
    D9 -> servo signal

  L293D: motor between pins 3 and 6; pin 16 to 5V logic;
  pin 8 to a supply suitable for the motor; pins 4,5,12,13 to GND.
  All power supplies and the Arduino must share a common ground.
  Use a suitable servo supply. Do not power the DC motor from an I/O pin.
  LEDs require series resistors (e.g. 330 ohms).
*/

#include <Arduino.h>
#include <Servo.h>

const byte JOY_X = A0, JOY_Y = A1;
const byte TRIG = 2, ECHO = 3;
const byte LEFT_LED = 4, RIGHT_LED = 12;
const byte MOTOR_ENABLE = 5, BUZZER = 6;
const byte MOTOR_IN1 = 7, MOTOR_IN2 = 8, SERVO_PIN = 9;

// Adjust centers using the raw X/Y values in the Serial Monitor.
const int X_CENTER = 512, Y_CENTER = 512;
const int DEAD_ZONE = 70;
const bool INVERT_X = false, INVERT_Y = true;
const int STEER_LEFT = 45, STEER_CENTER = 90, STEER_RIGHT = 135;
const float BRAKE_CM = 20.0, RELEASE_CM = 25.0;
const unsigned long RANGE_INTERVAL_MS = 60;
const unsigned long ECHO_TIMEOUT_US = 25000;
const unsigned long BLINK_MS = 350, REPORT_MS = 250;
const unsigned long REVERSE_PAUSE_MS = 150;

Servo steering;
int rawX = 512, rawY = 512;
int throttle = 0, steerCommand = 0, steerAngle = STEER_CENTER;
int actualMotor = 0;  // -255 = full reverse; +255 = full forward.
int lastDirection = 0;
unsigned long stoppedAt = 0;
float distanceCm = -1;
bool rangeValid = false, brakeActive = true;
bool blinkOn = false;
int previousTurn = 0;
unsigned long lastRange = 0, lastBlink = 0, lastReport = 0;

// Converts an analog axis to -255..255 with a neutral region.
int readAxis(int raw, int center, bool invert) {
  int value = 0;
  if (raw > center + DEAD_ZONE) {
    value = map(raw, center + DEAD_ZONE, 1023, 0, 255);
  } else if (raw < center - DEAD_ZONE) {
    value = map(raw, 0, center - DEAD_ZONE, -255, 0);
  }
  value = constrain(value, -255, 255);
  return invert ? -value : value;
}

void readJoystick() {
  rawX = analogRead(JOY_X);
  rawY = analogRead(JOY_Y);
  throttle = readAxis(rawY, Y_CENTER, INVERT_Y);
  steerCommand = readAxis(rawX, X_CENTER, INVERT_X);
  if (steerCommand < 0) {
    steerAngle = map(steerCommand, -255, 0, STEER_LEFT, STEER_CENTER);
  } else {
    steerAngle = map(steerCommand, 0, 255, STEER_CENTER, STEER_RIGHT);
  }
  steering.write(steerAngle);
}

void updateDistance(unsigned long now) {
  if (now - lastRange < RANGE_INTERVAL_MS) return;
  lastRange = now;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // Bounds the wait so a missing echo cannot freeze the controls.
  unsigned long duration = pulseIn(ECHO, HIGH, ECHO_TIMEOUT_US);
  distanceCm = duration == 0 ? -1.0 : duration * 0.0343 / 2.0;
  rangeValid = duration != 0;

  if (!rangeValid || distanceCm <= BRAKE_CM) {
    brakeActive = true;
  } else if (distanceCm >= RELEASE_CM) {
    brakeActive = false;
  }
  // Between the two limits, retain the previous brake state.
}

void brakeMotor(unsigned long now) {
  // Disable output while changing the input states, then actively brake.
  analogWrite(MOTOR_ENABLE, 0);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_ENABLE, HIGH);
  if (actualMotor != 0) stoppedAt = now;
  actualMotor = 0;
}

void updateMotor(unsigned long now) {
  int requested = brakeActive ? 0 : throttle;
  int direction = requested > 0 ? 1 : (requested < 0 ? -1 : 0);

  if (direction == 0) {
    brakeMotor(now);
    return;
  }

  if (lastDirection != 0 && direction != lastDirection) {
    // Stop first, then wait before energizing the opposite direction.
    if (actualMotor != 0) brakeMotor(now);
    if (now - stoppedAt < REVERSE_PAUSE_MS) return;
  }

  analogWrite(MOTOR_ENABLE, 0);
  digitalWrite(MOTOR_IN1, direction > 0 ? HIGH : LOW);
  digitalWrite(MOTOR_IN2, direction > 0 ? LOW : HIGH);
  analogWrite(MOTOR_ENABLE, abs(requested));
  actualMotor = requested;
  lastDirection = direction;
}

void updateIndicators(unsigned long now) {
  int turn = steerCommand < 0 ? -1 : (steerCommand > 0 ? 1 : 0);

  if (turn != previousTurn) {
    blinkOn = turn != 0;
    lastBlink = now;
    previousTurn = turn;

  } else if (turn != 0 && now - lastBlink >= BLINK_MS) {
    blinkOn = !blinkOn;
    lastBlink = now;
  }
  digitalWrite(LEFT_LED, turn < 0 && blinkOn ? HIGH : LOW);
  digitalWrite(RIGHT_LED, turn > 0 && blinkOn ? HIGH : LOW);

  // Short recurring beeps while the obstacle brake or echo fault is active.
  if (brakeActive && now % 400UL < 150UL) tone(BUZZER, 2000);
  else noTone(BUZZER);
}

void reportStatus(unsigned long now) {
    if (now - lastReport < REPORT_MS) return;
    lastReport = now;

    Serial.print(F("X="));
    Serial.print(rawX);
    Serial.print(F(" Y="));
    Serial.print(rawY);
    Serial.print(F(" | requested="));
    Serial.print(throttle);
    Serial.print(F(" | motion="));

    if (actualMotor > 0) {
        Serial.print(F("FORWARD"));
    } else if (actualMotor < 0) {
        Serial.print(F("REVERSE"));
    } else {
        Serial.print(F("STOPPED"));
    }

    Serial.print(F(" | PWM="));
    Serial.print(abs(actualMotor));
    Serial.print(F(" | speed%="));
    Serial.print(abs(actualMotor) * 100L / 255);
    Serial.print(F(" | angle="));
    Serial.print(steerAngle);
    Serial.print(F(" | turn="));

    if (steerCommand < 0) {
        Serial.print(F("LEFT"));
    } else if (steerCommand > 0) {
        Serial.print(F("RIGHT"));
    } else {
        Serial.print(F("STRAIGHT"));
    }

    Serial.print(F(" | distance_cm="));

    if (rangeValid) {
        Serial.print(distanceCm, 1);
    } else {
        Serial.print(F("NO_ECHO"));
    }

    Serial.print(F(" | brake="));

    if (!rangeValid) {
        Serial.println(F("ON (echo missing)"));
    } else if (brakeActive) {
        Serial.println(F("ON (obstacle)"));
    } else {
        Serial.println(F("OFF"));
    }

    // speed% reports commanded PWM duty, not measured motor RPM.
}

void setup() {
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  brakeMotor(millis());
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, LOW);
  pinMode(ECHO, INPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(STEER_CENTER);
  Serial.begin(9600);
  Serial.println(F("Toy Car Simulator ready. Center joystick to stop."));
}

void loop() {
  readJoystick();
  updateDistance(millis());
  unsigned long now = millis();
  updateMotor(now);
  updateIndicators(now);
  reportStatus(now);
}
