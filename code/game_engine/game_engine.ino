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

byte bird[] = {
 B00000,
 B00000,
 B00000,
 B11100,
 B10111,
 B01111,
 B00100,
 B00000
};
 
byte dinosaur[] = {
  B00000,
  B01111,
  B01011,
  B01111,
  B01110,
  B11111,
  B11101,
  B10100
};

int buttonPin = 2;
const int mapLength = 16;
int position = 16;
int objectMap[mapLength];
bool state = LOW;
bool currentlyJumping = LOW;
bool gameOver = LOW;
int jumpCounter = 3;
int score = 0;

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
void setup(){
  pinMode(buttonPin, INPUT);
  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.createChar(0, smallCactus);
  lcd.createChar(1, bigCactus);
  lcd.createChar(2, bird);
  lcd.createChar(3, dinosaur);
  lcd.home();
  Serial.begin(9600);
  lcd.setCursor(2, 1);
  lcd.write(byte(3));
}

void loop(){ 
  input();
  if(millis() % 500 == 0 && !gameOver) {
    jump();
    if(position < 16) {
      removeObstacle(position, 1);
    }
    position--;
    displayObstacle(0, position, 1);
    if(!currentlyJumping && objectMap[2] == 1) {
      endPlayerScore(score);
    }
    if(!currentlyJumping && !gameOver) {
      displayObject(3, 2, 1);
    }
  }
}

void input() {
  if(position < 1) {
    delay(200);
    removeObstacle(position, 1);
    position = mapLength;
  }
  state = digitalRead(buttonPin);
  if(state && !currentlyJumping) {
    currentlyJumping = HIGH;
  }
}

void jump() {
  if(currentlyJumping) {
    if(jumpCounter == 3) {
      removeObject(2, 1);
      displayObject(3, 2, 0);
    } else if(jumpCounter == 1) {
      removeObject(2, 0);
      displayObject(3, 2, 1);
    }
    printMap();
    jumpCounter--;
  }
  if(jumpCounter == 0) {
    state = LOW;
    currentlyJumping = LOW;
    jumpCounter = 3;
  }
  score++;
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

void displayObject(int symbol, int position, int level) {
  lcd.setCursor(position, level);
  lcd.write(byte(symbol));
}

void removeObject(int position, int level) {
  lcd.setCursor(position, level);
  lcd.print(" ");
}

void printMap() {
  Serial.print(String(position) + " ");
  for(int i = 0; i < mapLength; i++) {
    Serial.print(objectMap[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void endPlayerScore(int points) {
  gameOver = HIGH;
  removeObstacle(2, 1);
  lcd.setCursor(3,0);
  lcd.print("GAME OVER!");
  lcd.setCursor(3,1);
  lcd.print("SCORE = " + String(points));
}

