// C++ code
//
#include <Adafruit_LiquidCrystal.h>

#include <Servo.h>

int nappiPainettuna = 0;

int painallusLaskuri = 0;

int nollausLaskuri = 0;

int SILMUKAN_TOISTOAIKA_MS = 0;

Adafruit_LiquidCrystal lcd_1(0);

Servo servo_3;

void setup()
{
  lcd_1.begin(16, 2);
  servo_3.attach(3, 500, 2500);
  pinMode(2, INPUT);
  pinMode(11, OUTPUT);

  // Vakiot
  SILMUKAN_TOISTOAIKA_MS = 50;
  // Alusta LCD-näyttö.
  lcd_1.setBacklight(1);
  lcd_1.setCursor(4, 0);
  lcd_1.print("LUKITTU");
  // Nollaa lukon asento.
  servo_3.write(0);
}

void loop()
{
  painallusLaskuri = 0;
  while (painallusLaskuri < 3) {
    if (digitalRead(2) == LOW) {
      if (nappiPainettuna == 0) {
        nappiPainettuna = 1;
        painallusLaskuri += 1;
        // Nollaa laskuri aina kun nappia on painettu.
        nollausLaskuri = (650 / SILMUKAN_TOISTOAIKA_MS);
      }
    } else {
      nappiPainettuna = 0;
    }
    if (nollausLaskuri > 0) {
      nollausLaskuri += -1;
      if (nollausLaskuri == 0) {
        // Laskuri on nollassa. Nollaa painallusten
        // laskuri.
        painallusLaskuri = 0;
      }
    }
    delay(SILMUKAN_TOISTOAIKA_MS); // Wait for SILMUKAN_TOISTOAIKA_MS millisecond(s)
  }
  // Avaa lukko.
  servo_3.write(90);
  // Tulosta "auki" LCD-näytölle.
  lcd_1.clear();
  lcd_1.setCursor(6, 0);
  lcd_1.print("AUKI");
  // Käynnistä summeri.
  analogWrite(11, 127);
  delay(3000); // Wait for 3000 millisecond(s)
  // Sammuta summeri.
  analogWrite(11, 0);
  delay(5000); // Wait for 5000 millisecond(s)
  // Sulje lukko.
  servo_3.write(0);
  delay(1000); // Wait for 1000 millisecond(s)
  // Tulosta "lukittu" LCD-näytölle.
  lcd_1.clear();
  lcd_1.setCursor(4, 0);
  lcd_1.print("LUKITTU");
}