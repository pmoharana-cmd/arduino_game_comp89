#include <Wire.h>
#include <LiquidCrystal_I2C.h>

byte bigCactus[] = {
  B00100,
  B00101,
  B10101,
  B10101,
  B10101,
  B10111,
  B11100,
  B00100
};

byte smallCactus[] = {
  B00000,
  B00000,
  B00100,
  B00101,
  B10101,
  B10111,
  B11100,
  B00100
};

int position = 15;

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup(){
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.createChar(0, smallCactus);
  lcd.home();
}
void loop(){
  lcd.setCursor(position, 1);
  lcd.write(byte(0));
  delay(500);
  lcd.setCursor(position, 1);
  lcd.print(" ");
  position--;
  if(position < 0) {
    position = 15;
  }
}

