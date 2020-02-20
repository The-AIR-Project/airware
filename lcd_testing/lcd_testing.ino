#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();

}


void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Squeeze");
  delay(750);
  lcd.clear();
  delay(750);
  lcd.setCursor(0,1);
  lcd.print("my bag");
  delay(750);
  lcd.clear();
  delay(750);
  lcd.setCursor(0,0);
  lcd.print("Squeeze");
  lcd.setCursor(0,1);
  lcd.print("my bag");
  delay(1500);
  lcd.clear();
  delay(750);
  
}
