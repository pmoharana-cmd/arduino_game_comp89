#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Initializes binary arrarys to create custom characters
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

//intialize all variables that will be used
int buttonPin = 2;
const int mapLength = 16;
int position = 16;
int objectMap[mapLength]; //legacy and for testing
int numOfObjects = 6;
bool firstRun[] = {LOW, HIGH, HIGH, HIGH, HIGH, HIGH};
int map1[] = {16, 12, 11, 8, 5, 1};
int objects[] = {0, 1, 0, 1, 2, 1, 2};
bool state = LOW;
bool currentlyJumping = LOW;
bool gameOver = LOW;
int jumpCounter = 3;
int score = 0;
int speed = 1000;

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
  randomSeed(analogRead(0));
}

void loop(){ 
  input(); //inputs are tracked in this function. Runs every tick
  gameEngine(); //calculations and actuations happen side by side. Runs every certain amount of ticks determined by the speed variable.
}

void input() { //gets the input from the button and runs every tick
  for(int i = 0; i < numOfObjects; i++) { //removes objects as they hit the end of the screen
    if(map1[i] < 1) {
      delay(200);
      removeObstacle(map1[i], 1);
      map1[i] = mapLength;
      objects[i] = random(0, 3); //randomly changes obstacle after first cycle
    }
  } 
  state = digitalRead(buttonPin);
  if(state && !currentlyJumping) { //makes it so the button cannot change state of currentlyJumping unless the the dinosaur has stopped jumping
    currentlyJumping = HIGH;
  }
}

void jump() { //changes the dinosaur's position based on currently jumping and jumpCounter
  if(currentlyJumping) {
    if(jumpCounter == 3) { //moves the dinosaur up
      removeObject(2, 1);
      displayObject(3, 2, 0);
    } else if(jumpCounter == 1) { //moves the dinosaur down
      removeObject(2, 0);
      displayObject(3, 2, 1);
    }
    jumpCounter--; //decrements the jump counter every time the game moves
  }
  if(jumpCounter == 0) { //resets variables after jump is completed
    state = LOW;
    currentlyJumping = LOW;
    jumpCounter = 3;
  }
  score++; //increases the score at the end of each tick
}

void gameEngine() { //main game engine code that keeps track of object positions and ends game if collision is detected
  if(millis() % speed == 0 && !gameOver) {
    jump(); //runs all the code for checks if it is jumping
    for(int i = 0; i < numOfObjects; i++) { //runs a loop for all the objects
      if(map1[i] < 16 && !firstRun[i]) { //removes the obstacle from the previous cycle
        removeObstacle(map1[i], 1);
      }
      if(!firstRun[i]) { //displays obstacles for the current cycle and decrements the positions in the position map
        map1[i] = map1[i] - 1;
        displayObstacle(objects[i], map1[i], 1);
      } else {
        map1[i] = map1[i] + 1;
        if(map1[i] == 16) { //does checking to allow the first couple obstacles to come in gradually
          firstRun[i] = LOW;
        }
      }
    }
    if(!currentlyJumping && objectMap[2] == 1) { //if collision is detected run the endPlayerScore() function
      endPlayerScore(score);
    }
    if(!currentlyJumping && !gameOver) { //resets the dinosaur position if not jumping to ensure it is always there (address bug that caused the dinosaur to disappear in a specific jump case)
      displayObject(3, 2, 1);
    }
    updateScore();
  }
}

void updateScore() { //updates the score on the display during gameplay
  if(!gameOver) {
    if(score < 10) { //changes the position based on the score amount
      lcd.setCursor(15, 0);
      lcd.print(score);
    } else if(score < 100) {
      lcd.setCursor(14, 0);
      lcd.print(score);
    } else if(score < 1000) {
      lcd.setCursor(13, 0);
      lcd.print(score);
    } else {
      lcd.setCursor(12, 0);
      lcd.print(score);
    }
  }
  if(score % 16 == 0 && speed > 200) { //updates speed every 16 points
    speed = speed - 100;
  }
}

void displayObstacle(int symbol, int position, int level) { //displays an object given a symbol value, position, and level (Does impact map)
  lcd.setCursor(position, level);
  lcd.write(byte(symbol));
  //objectMap[position] = 1; code used for previous version 
}

void removeObstacle(int position, int level) { //removes an object given a position and level (Does not impact map)
  lcd.setCursor(position, level);
  lcd.print(" ");
  //objectMap[position] = 0; code used for previous version
}

void displayObject(int symbol, int position, int level) { //displays an object given a symbol value, position, and level (Does not impact map)
  lcd.setCursor(position, level);
  lcd.write(bytesymbol));
}

void removeObject(int position, int level) { //removes an object given a position and level (Does not impact map)
  lcd.setCursor(position, level);
  lcd.print(" ");
}

void printMap() { //used for debugging purposes to see map and where the objects were (No longer functional)
  Serial.print(String(position) + " ");
  for(int i = 0; i < mapLength; i++) {
    Serial.print(objectMap[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void endPlayerScore(int points) { //ends the game and displays the score
  gameOver = HIGH;
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("GAME OVER!");
  lcd.setCursor(3,1);
  lcd.print("SCORE = " + String(points));
}

