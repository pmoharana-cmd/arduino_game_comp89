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

const int mapLength = 16;
int position = 15;

int objectMap[mapLength];

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup(){
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.createChar(0, bigCactus);
  lcd.home();
  Serial.begin(9600);
}

void loop(){
  if(position < 0) {
    position = mapLength - 1;
  }
  displayObstacle(0, position, 1);
  printMap();
  delay(1000);
  removeObstacle(position, 1);
  position--;
}

void gameEngine() {
  
}

void displayObstacle(int symbol, int position, int level) {
  lcd.setCursor(position, level);
  lcd.write(byte(symbol));
  objectMap[position] = 1;
}

void removeObstacle(int position, int level) {
  lcd.setCursor(position, level);
  lcd.print(" ");
  objectMap[position] = 0;
}

void printMap() {
  Serial.print(String(position) + " ");
  for(int i = 0; i < mapLength; i++) {
    Serial.print(objectMap[i]);
    Serial.print(" ");
  }
  Serial.println();
}

