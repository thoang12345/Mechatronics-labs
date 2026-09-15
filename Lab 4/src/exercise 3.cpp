#include <Arduino.h>
#include <LiquidCrystal.h>

// LCD pins
const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Button pins
const int startPin = 6;
const int lapPin = 7;

// Stopwatch state
bool timing = false;

unsigned long startTime = 0;
unsigned long previousTime = 0;
unsigned long elapsedTime = 0;
unsigned long oldTime = 0;
unsigned long lapTime = 0;

void setup() {
    lcd.begin(16, 2);

    pinMode(startPin, INPUT);
    pinMode(lapPin, INPUT);

    // Initial screen
    lcd.setCursor(0, 0);
    lcd.print("Stopwatch");

    lcd.setCursor(0, 1);
    lcd.print("Press Start");
}

void loop() {
    int startVal = digitalRead(startPin);

    // Start or resume the stopwatch
    if (!timing && startVal == HIGH) {
        delay(50);  // debounce

        if (digitalRead(startPin) == HIGH) {
            lcd.clear();

            startTime = millis();
            timing = true;

            // Wait for button release so one press does not immediately pause it
            while (digitalRead(startPin) == HIGH) {
                delay(10);
            }
        }
    }

    // Update stopwatch while running
    if (timing) {
        elapsedTime = previousTime + (millis() - startTime);

        unsigned long totalSeconds = elapsedTime / 1000;
        unsigned long minutes = totalSeconds / 60;
        unsigned long seconds = totalSeconds % 60;

        char timeLine[17];
        snprintf(timeLine, sizeof(timeLine), "%02lu:%02lu", minutes, seconds);

        lcd.setCursor(0, 0);
        lcd.print("Time: ");
        lcd.print(timeLine);
        lcd.print("     ");  // clears leftover characters

        // Check lap button
        if (digitalRead(lapPin) == HIGH) {
            delay(50);  // debounce

            if (digitalRead(lapPin) == HIGH) {
                lapTime = elapsedTime - oldTime;
                oldTime = elapsedTime;

                unsigned long lapSecondsTotal = lapTime / 1000;
                unsigned long lapMinutes = lapSecondsTotal / 60;
                unsigned long lapSeconds = lapSecondsTotal % 60;

                char lapLine[17];
                snprintf(lapLine, sizeof(lapLine),
                         "Lap %02lu:%02lu",
                         lapMinutes, lapSeconds);

                lcd.setCursor(0, 1);
                lcd.print(lapLine);
                lcd.print("    ");

                // Wait for lap-button release
                while (digitalRead(lapPin) == HIGH) {
                    delay(10);
                }
            }
        }

        // Pause stopwatch when Start is pressed again
        if (digitalRead(startPin) == HIGH) {
            delay(50);  // debounce

            if (digitalRead(startPin) == HIGH) {
                previousTime = elapsedTime;
                timing = false;

                while (digitalRead(startPin) == HIGH) {
                    delay(10);
                }
            }
        }
    }
}