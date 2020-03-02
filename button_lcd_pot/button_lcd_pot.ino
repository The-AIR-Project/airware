#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
int potValueVol = 0;
int potValueRate = 0;
int potValuePress = 0;
int buttonState = 0;
char *settings[3] = {"Adult", "Child", "Infant"};
char clearLine[] = "                    ";
bool canReadButton = true;
long buttonTime = 0;

void printAgeSetting()
{
  lcd.setCursor(0,0);
  lcd.print(clearLine);
  lcd.setCursor(0,0);
  lcd.print("Age Setting: ");
  lcd.setCursor(13, 0);
  lcd.print(settings[buttonState]);
}

void printVolSetting()
{
  lcd.setCursor(0,1);
  lcd.print(clearLine);
  lcd.setCursor(0,1);
  lcd.print("Volume: ");
  lcd.setCursor(13, 1);
  lcd.print(potValueVol);
}

void printPressSetting()
{
  lcd.setCursor(0,2);
  lcd.print(clearLine);
  lcd.setCursor(0,2);
  lcd.print("Pressure: ");
  lcd.setCursor(13, 2);
  lcd.print(potValuePress);
}

void printRateSetting()
{
  lcd.setCursor(0,3);
  lcd.print(clearLine);
  lcd.setCursor(0,3);
  lcd.print("Breath Rate: ");
  lcd.setCursor(13, 3);
  lcd.print(potValueRate);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  printAgeSetting();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(8) == 0 && canReadButton) {
    buttonTime = millis();
    canReadButton = false;
    buttonState += 1;
    buttonState = buttonState % 3;
    printAgeSetting();
  }
  if (!canReadButton && millis() - buttonTime >= 500) {
    canReadButton = true;
  }

  if (map(analogRead(0), 1024, 0, 0, 10) != potValueVol) {
    potValueVol = map(analogRead(0), 1024, 0, 0, 10);
    printVolSetting();
  }

  if (map(analogRead(0), 1024, 0, 0, 20) != potValuePress) {
    potValuePress = map(analogRead(0), 1024, 0, 0, 20);
    printPressSetting();
  }

  if (map(analogRead(0), 1024, 0, 0, 30) != potValueRate) {
    potValueRate = map(analogRead(0), 1024, 0, 0, 30);
    printRateSetting();
  }
}
