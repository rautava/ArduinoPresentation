#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Constants
const int BUTTON_PIN = 2;
const int SERVO_PIN = 3;
const int BUZZER_PIN = 11;

const int LOOP_INTERVAL_MS = 50;
const int CLICK_INTERVAL_MS = 650;
const int PLAY_BUZZER_LOCK_MS = 100;
const int PLAY_BUZZER_UNLOCK_MS = 3000;
const int LOCK_AFTER_MS = 5000;

const int RESET_COUNTER_INITIAL_VALUE = CLICK_INTERVAL_MS / LOOP_INTERVAL_MS;

const char* LOCKED_TEXT = "LUKITTU";
const char* UNLOCKED_TEXT = "AUKI";

const int SERVO_LOCKED_VALUE = 90;
const int SERVO_UNLOCKED_VALUE = 0;
const int SERVO_WAIT_TIME_MS = 500;

const int LCD_I2C_ADDR = 0x27;
const int LCD_COLS = 16;
const int LCD_ROWS = 2;
const int LOCKED_TEXT_COL = (LCD_COLS - strlen(LOCKED_TEXT)) / 2;
const int UNLOCKED_TEXT_COL = (LCD_COLS - strlen(UNLOCKED_TEXT)) / 2;

const int REQUIRED_BUTTON_PRESS_COUNT = 3;

LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);
Servo servo;

void turnOnBuzzer() {
  analogWrite(BUZZER_PIN, 127);
}

void turnOffBuzzer() {
  analogWrite(BUZZER_PIN, 0);
}

void unlock() {
  turnOnBuzzer();

  servo.write(SERVO_UNLOCKED_VALUE);
  delay(SERVO_WAIT_TIME_MS);

  lcd.clear();
  lcd.setCursor(UNLOCKED_TEXT_COL, 0);
  lcd.print(UNLOCKED_TEXT);

  delay(abs(PLAY_BUZZER_UNLOCK_MS - SERVO_WAIT_TIME_MS));
  turnOffBuzzer();  
}

void lock() {
  servo.write(SERVO_LOCKED_VALUE);

  turnOnBuzzer();
  delay(PLAY_BUZZER_LOCK_MS);
  turnOffBuzzer();  

  delay(abs(SERVO_WAIT_TIME_MS - PLAY_BUZZER_LOCK_MS));

  lcd.clear();
  lcd.setCursor(LOCKED_TEXT_COL, 0);
  lcd.print(LOCKED_TEXT);
}

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  servo.attach(SERVO_PIN);

  lcd.init();
  lcd.setBacklight(1);

  lock();
}

void loop()
{
  bool buttonPressed = true;
  int clickCounter = 0;
  int resetCounter = 0;

  while (clickCounter < REQUIRED_BUTTON_PRESS_COUNT) {
    if (digitalRead(BUTTON_PIN) == LOW) {

      if (!buttonPressed) {
        buttonPressed = true;
        ++clickCounter;
        resetCounter = RESET_COUNTER_INITIAL_VALUE;
      }
    } else {
      buttonPressed = 0;
    }

    if (resetCounter > 0) {
      --resetCounter;

      if (resetCounter <= 0) {
        clickCounter = 0;
      }
    }

    delay(LOOP_INTERVAL_MS);
  }

  unlock();
  delay(LOCK_AFTER_MS);
  lock();
}