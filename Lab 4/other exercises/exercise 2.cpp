#include <Arduino.h>
#include <LiquidCrystal.h>

const int rs=12, en=11, d4=5, d5=4, d6=3,d7=2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int reply;

void setup() {
    Serial.begin(9600);

    lcd.begin(16, 2);
    pinMode(switchPin, INPUT);

    lcd.setCursor(0, 0);
    lcd.print("Ask the");
    lcd.setCursor(0, 1);
    lcd.print("Crystal Ball!");

    delay(100);  
    // allow tilt sensor signal to stabilize
    // An early signal was causing the program to 
    // think the tilt swtich was triggered
    // when it was not.
    prevSwitchState = digitalRead(switchPin);
}

void loop() {
    switchState = digitalRead(switchPin);
    Serial.println("Switch state: " + String(switchState)   + " Previous state: " + String(prevSwitchState));
    if (switchState != prevSwitchState) {
        if (switchState == HIGH) {
            reply = random(8);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("The ball says:");
            lcd.setCursor(0,1);
            //here are all the possible responses that the 
            //crystal ball can give
            switch (reply) {
                case 0:
                    lcd.print("Yes!");
                    break;
                case 1:
                    lcd.print("No!");
                    break;
                case 2:
                    lcd.print("Maybe!");
                    break;
                case 3:
                    lcd.print("Ask again");
                    break;
                case 4:
                    lcd.print("Definitely!");
                    break;
                case 5:
                    lcd.print("Unsure");
                    break;
                case 6:
                    lcd.print("Doubtful");
                    break;
                case 7:
                    lcd.print("Outlook good!");
                    break;
            }
        }
        delay(50); // Debounce delay
    }
    prevSwitchState = switchState;
}