//F1 Race
//Francois W. Nel
//2015-02-20
//Creative Commons BY-NC-SA
//
//Requirements:
//Standard 16x2 LCD
//Push buttons connected to pins 6, 7

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int rightButtonPin = 6;
int leftButtonPin = 7;
int rightButtonState, leftButtonState, chance, gameSpeed;
bool steerLeft, steerRight;

//Sprites
byte blank[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

//Car sprite split across 3 characters
byte car1[8] = {
  0b11100,
  0b11101,
  0b01001,
  0b11111,
  0b11111,
  0b01001,
  0b11101,
  0b11100
};

byte car2[8] = {
  0b00000,
  0b01100,
  0b11100,
  0b11111,
  0b11111,
  0b11100,
  0b01100,
  0b00000
};

byte car3[8] = {
  0b00111,
  0b00111,
  0b11010,
  0b11111,
  0b11111,
  0b11010,
  0b00111,
  0b00111
};

void setup() {
  lcd.begin(16, 2);
  lcd.clear();

  //Create sprites
  lcd.createChar(0, blank);
  lcd.createChar(1, car1);
  lcd.createChar(2, car2);
  lcd.createChar(3, car3);

  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);

  //Game speed in milliseconds
  gameSpeed = 75;

  //Title screen
  lcd.setCursor(7, 0);
  lcd.print("F1");
  lcd.setCursor(6, 1);
  lcd.print("RACE");
  delay(1500);
  lcd.clear();
  
  //Initial car position
  steerLeft = true;
  steerRight = false;
}

void loop() {
  //Check for steering state
  steering();

  //Generate other drivers randomly. Used 0-9 to experiment with different probabilities
  chance = random(9);
  if (chance < 5) {
    leftLane();
  }
  if (chance >= 5) {
    rightLane();
  }
}

void steering() {
  //Read button state
  leftButtonState = digitalRead(leftButtonPin);
  rightButtonState = digitalRead(rightButtonPin);
  
  //Left button pushed
  if (leftButtonState == LOW) {
    steerLeft = true;
    steerRight = false;
  }
  
  //Right button pushed
  if (rightButtonState == LOW) {
    steerLeft = false;
    steerRight = true;
  }
  
  //Draw car in left lane
  if (steerLeft) {
    lcd.setCursor(2,0);
    lcd.write(3);
    lcd.setCursor(3,0);
    lcd.write(2);
    lcd.setCursor(4,0);
    lcd.write(1);
    lcd.setCursor(2,1);
    lcd.write(byte(0));
    lcd.setCursor(3,1);
    lcd.write(byte(0));
    lcd.setCursor(4,1);
    lcd.write(byte(0));
  }
  
  //Draw car in right lane
  if (steerRight) {
    lcd.setCursor(2,0);
    lcd.write(byte(0));
    lcd.setCursor(3,0);
    lcd.write(byte(0));
    lcd.setCursor(4,0);
    lcd.write(byte(0));
    lcd.setCursor(2,1);
    lcd.write(3);
    lcd.setCursor(3,1);
    lcd.write(2);
    lcd.setCursor(4,1);
    lcd.write(1);
  }
}

void leftLane() {
  //Scroll other drivers in left lane
  for (int i = 15; i >= 0; i--) {
    lcd.setCursor(i, 0);
    lcd.write(3);
    lcd.setCursor(i + 1, 0);
    lcd.write(2);
    lcd.setCursor(i + 2, 0);
    lcd.write(1);
    lcd.setCursor(i + 3, 0);
    lcd.write(byte(0));
    lcd.setCursor(0, 0); //Dirty fix for sprites remaining on screen, intend to fix with a better loop
    lcd.write(byte(0));
    lcd.setCursor(1, 0); //Same as above
    lcd.write(byte(0));
    delay(gameSpeed);

    //Check for game over state
    if (i == 4 && steerLeft) {
      gameOver();
    }

    //Continuously check steering state
    steering();
  }
}

void rightLane() {
  //Scroll other drivers in right lane
  for (int i = 15; i >= 0; i--) {
    lcd.setCursor(i, 1);
    lcd.write(3);
    lcd.setCursor(i + 1, 1);
    lcd.write(2);
    lcd.setCursor(i + 2, 1);
    lcd.write(1);
    lcd.setCursor(i + 3, 1);
    lcd.write(byte(0));
    lcd.setCursor(0, 1); //Dirty fix for sprites remaining on screen, intend to fix with a better loop
    lcd.write(byte(0));
    lcd.setCursor(1, 1); //Same as above
    lcd.write(byte(0));
    delay(gameSpeed);

    //Check for game over state
    if (i == 4 && steerRight) {
      gameOver();
    }

    //Continuously check steering state
    steering();
  }
}

void gameOver() {
  //Game over screen, results in infinite loop until the Arduino is reset
  lcd.clear();
  while (true) {
    lcd.setCursor(6, 0);
    lcd.print("GAME");
    lcd.setCursor(6, 1);
    lcd.print("OVER");
  }
}
