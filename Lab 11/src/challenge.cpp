#include <Arduino.h>
#include <Stepper.h>
#include <IRremote.hpp>

const int stepsPerRevolution = 2048;
const int IR_RECEIVE_PIN = 7;

const int minSpeed = 1;
const int maxSpeed = 15;

int speed = 10;
bool running = false;
bool direction = true;

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  myStepper.setSpeed(speed);

  Serial.println("Ready. Press Play/Pause to start.");
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    bool repeat =
        IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT;

    IrReceiver.resume();

    // Ignore held-button repeats so pause toggles only once
    if (!repeat) {
      bool recognized = true;

      switch (command) {
        // Play/Pause: toggle running
        case 64:
          running = !running;
          break;

        // Next: clockwise
        case 67:
          direction = true;
          break;

        // Back: counterclockwise
        case 68:
          direction = false;
          break;

        // Vol+: increase speed by 1 RPM
        case 70:
          speed = constrain(speed + 1, minSpeed, maxSpeed);
          break;

        // Vol-: decrease speed by 1 RPM
        case 21:
          speed = constrain(speed - 1, minSpeed, maxSpeed);
          break;

        // Number buttons: preset speeds in RPM
        case 22:  // 0
          speed = 1;
          break;

        case 12:  // 1
          speed = 2;
          break;

        case 24:  // 2
          speed = 3;
          break;

        case 94:  // 3
          speed = 5;
          break;

        case 8:   // 4
          speed = 6;
          break;

        case 28:  // 5
          speed = 8;
          break;

        case 90:  // 6
          speed = 9;
          break;

        case 66:  // 7
          speed = 11;
          break;

        case 82:  // 8
          speed = 13;
          break;

        case 74:  // 9
          speed = 15;
          break;

        default:
          recognized = false;
          Serial.print("Unassigned command: ");
          Serial.println(command);
          break;
      }

      if (recognized) {
        myStepper.setSpeed(speed);

        Serial.print(running ? "Running" : "Paused");
        Serial.print(" | Speed: ");
        Serial.print(speed);
        Serial.print(" RPM | Direction: ");
        Serial.println(direction ? "Clockwise" : "Counterclockwise");
      }
    }
  }

  // Move one step at a time so the remote stays responsive
  if (running) {
    myStepper.step(direction ? 1 : -1);
  }
}