#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <RGBmatrixPanel.h>

//Arduino Mega Pin Definition
#define A A0              // ROW SELECT: WHICH TWO ROWS CURRENTLY LIT
#define B A1
#define C A2
#define D A3

#define MotorPin 5
#define buzzer 7
#define OE 9              // OUTPUT ENABLE
#define LAT 10            // LATCH SIGNAL MARKS THE END OF A ROW OF DATA
#define CLK 11            // USE THIS ON ARDUINO MEGA
#define betBtn 18
#define spinBtn 19
#define isObstaclePin 2
#define reservePin 15

#define R1 24             // UPPER RGB DATA - TOP HALF OF DISPLAY
#define G1 25
#define B1 26
#define R2 27             // LOWER RGB DATA - BOTTOM HALF OF DISPLAY
#define G2 28
#define B2 29
#define pi 3.1415926535897932384626433832795

//Music
int musicCounter;
int fireworkMusic[] = {622, 784, 932, 1244, 1175, 932, 784, 1047, 932, 784, 622, 932, 783, 784, 831, 932, 1046, 784, 698, 1046, 932, 784, 622, 932, 831, 784, 699, 698, 622};
int rollingMusic[] = {523, 622, 784, 622, 466, 622, 784, 622};
int fallingNotesMusic[] = {784, 622, 523};

//Array of Combos & Select, Combinations of all possible Frames (10)
String combo[] = {"EEE", "NBS", "IEM", "ADM", "SCE", "NBS", "SCE", "ADM", "EEE", "SCE", "AD,", "NBS", "SCE", "ADM", "SCE", "NBS", "SCE", "ADM", "NBS", "SCE"};

//Array to determine scroll speed
//ySpeed, or scroll speed is proportional to weights (20)
float spdWeights[] = {0.6, 0.7, 0.8, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.4, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5};

//CONSTRUCTOR FOR 64x32 LED MATRIX PANEL
// RGBmatrixPanel matrix_without_db(A, B, C, D, CLK, LAT, OE, false, 64);
// RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 64);
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 64);

//LED Global Variables
int numOfFrames = 10;
int numberOfRotations = 20;
const float ySpeedsConstant[] = {10, 7, 8};   //original ySpeed
float currentYSpeeds[] = {0, 0, 0};
bool isSpinning = true;                       //spin button to trigger status;
int startingFrame;
int endingFrame;

//LED DESIGN GLOBAL VARIABLE;
uint16_t blackColor = matrix.Color333(0, 0, 0);
uint16_t redColor = matrix.Color333(255, 0, 0);
uint16_t orangeColor = matrix.Color333(255,75,0);
uint16_t blueColor = matrix.Color333(0, 255, 245);
uint16_t greenColor = matrix.Color333(76, 255, 56);

//LED WINING CONDITION
const String Jackpot = "IEM";
const String Win = "EEE";

//LCD Display Variable
int isObstacle = HIGH;
LiquidCrystal_I2C lcd(0x27,20,4);       // Set the LCD address to 0x27 for a 20 chars and 4 line display

//Bet Button Variables
int betState = LOW;
bool betPressed = false;

//Spin Button Variables
int spinState = LOW;
bool spinPressed = false;

//Credit, Bet variable
int creditAmt =0, betAmt=0, currentBetAmt=0, maxBet=3;

//IR sensor variable
bool coinInsert = false;

//Server Motor Variable
Servo myservo;
const int minCoinsRequired = 3;
int totalCoinsInside = 3;
int servoStartAngle = 35;
int servoEndAngle = 0;
int winRate = 2;
int adminCoin =0;

//For Waterfall Animation
int waterfallColumns[32];
int waterfallStartingPos[32];
int waterfallRotations = 40;
uint16_t waterfallColor1 = matrix.Color333(0, 210, 84);
uint16_t waterfallColor2 = matrix.Color333(0, 202, 202);
uint16_t waterfallColor3 = matrix.Color333(161, 0, 202);

//For Radition Animation
int radiationRotations = 20;
uint16_t radiationColors[4] = {matrix.Color333(226, 1, 175), matrix.Color333(223, 2, 198),
                               matrix.Color333(122, 3, 220), matrix.Color333(4, 5, 217)};

//For Triangle Spinning Animation
int triangleRadius = 5;
int triangleNumberofRotations = 10;
uint16_t triangleColors[3] = {matrix.Color333(236, 91, 212), matrix.Color333(232, 81, 132),
                              matrix.Color333(228, 98, 72)};

//For Fireworks Animation
const int set1XStart = 4, set1XEnd = 17;
const int set2XStart = 25, set2XEnd = 38;
const int set3XStart = 46, set3XEnd = 59;
const int yMax = 20, yMin =2;
bool roll = true;

//For Rocket ANIMATION
uint16_t x0=3, x1=4, x2=6, x3=7, x4=10, x5=15, x6= 19;

uint16_t mainRck_EL_R1R = 12, mainRck_ES_R2R = 14, mainRck_EL_R3R = 16, mainRck_ES_R4R = 18, mainRck_EL_R5R = 20,
         mainRck_EL_Start = -15, mainRck_ES_Start = -13, mainRck_EL_Length = 3, mainRck_ES_Length = 1,
         mainRck_Rect_X = -11, mainRck_Rect_Y = 12, mainRck_Rect_W = 5, mainRck_Rect_H = 9,
         mainRck_CenTri_X1 = -11, mainRck_CenTri_X2 = -9,
         mainRck_TT_Y1 = 9, mainRck_TT_Y2 = 11, mainRck_BT_Y1 = 21, mainRck_BT_Y2 = 23,
         mainRck_RT_X1 = -6, mainRck_RT_Y1 = 12, mainRck_RT_X2 = -2, mainRck_RT_Y2 = 16,  mainRck_RT_Y3 = 20;

uint16_t leftRect_W = 4, leftRect_H = 5, leftRect_X = -61,
         leftRck_EL_Start = -63, leftRck_ES_Start = -62,
         leftRck_EL_Length = 1, leftRck_ES_Length = 0,
         leftRck_CenTri_X1 = -61, leftRck_CenTri_X2 = -60,
         leftRck_RT_X1=-57, leftRck_TT_X1 = -61, leftRck_TT_X2 = -60;

uint16_t leftRck_EL_R1R = 3, leftRck_EL_R2R = 5, leftRck_EL_R3R = 7,
         leftRck_EL_R4R = 24, leftRck_EL_R5R = 26, leftRck_EL_R6R = 28,
         leftRect_top_Y = 3, leftRect_bottom_Y =  24,
         leftRck_TT1_Y1 = 1, leftRck_TT1_Y2 = 2, leftRck_TT2_Y1 = 22, leftRck_TT2_Y2 = 23,
         leftRck_BT1_Y1 = 8, leftRck_BT1_Y2 = 9, leftRck_BT2_Y1 = 29, leftRck_BT2_Y2 = 30,
         leftRck_RT_Y1 = 3, leftRck_RT_X2 = -55, leftRck_RT_Y2 = 5, leftRck_RT_Y3 = 7,
         leftRck_RT_Y4 = 24, leftRck_RT_Y5 = 26, leftRck_RT_Y6 = 28;

uint16_t firstChar_X=-54, secondChar_X=-41, thirdChar_X=-28, Char_Y =9;

//loop flags
bool betButtonPressedFlag = false;
bool spinButtonPressedFlag = false;

void setup(){
  Serial.begin(9600);

  lcd.init();                                    // Initialize the LCD
  lcd.backlight();
  pinMode(isObstaclePin, INPUT);
  pinMode(betBtn, INPUT);
  pinMode(spinBtn, INPUT);

  matrix.begin();
  matrix.setTextSize(3);
  matrix.setTextWrap(false);

  // *MUST SEED using Analog input from UNUSED_PIN. analogRead(reservePin).
  randomSeed(analogRead(reservePin));

  //LCD Display "Insert Coin" Message
  LcdMessage(0);
  rocketAnimation();

  //Software Interrupt
  attachInterrupts();
  Serial.print("spinButtonPressedFlag = ");
  Serial.println(spinButtonPressedFlag);
}

void loop(){
  //firework();
  Serial.println("in loop");
  Serial.print("spinButtonPressedFlag = ");
  Serial.println(spinButtonPressedFlag);

  if(totalCoinsInside >= winRate){
    irSensorUpdate();
    if(creditAmt>0){    
      if(betButtonPressedFlag) { //CASE: BET BUTTON PRESSED
        betButtonPressedFlag = false;     
        LcdMessage(1);
        BetIncrementSFX(); //CAUSING LAG!!! no delay if this is commented out
      }else if(spinButtonPressedFlag){ //CASE: SPIN BUTTON PRESSED
          spinButtonPressedFlag = false;
          if(totalCoinsInside < (winRate * betAmt)){ //CASE: Need admin to top up coins
            LcdMessage(6);
            bool admin = true;
            while(admin){
              admin = AdminCoinInsert();
            }
            adminCoin =0;
          }else{
            LcdMessage(7);
            SpinActivateSFX();
            activateSpin();
          }
      }
    }
  } 
}

void clearAllFlags() {
  betButtonPressedFlag = false;
  spinButtonPressedFlag = false;
}

//IR SENSOR METHOD: CHECK FOR COIN DETECTION
void irSensorUpdate(){
 
  isObstacle = digitalRead(isObstaclePin);

  //TODO: IDLE STATE ANIMATION
  if(isObstacle == LOW) {                       // Obstacle detected
    coinInsert = true;
  } else if(isObstacle == HIGH && coinInsert){  // No Obstacle AND coinInsert == True
    updateCredit(1);                            // Incremeent Credit Amt'
    LcdMessage(1);                              // Display Message in LCD
    CoinInsertSFX();
    clearAllFlags();
  }
}

//BUTTON METHOD: DETERMINE WHICH BUTTON IS PRESSED
void betButtonPress(){
  if(creditAmt > 0) {
    betButtonPressedFlag = true;
    updateBet();
    BetIncrementSFX();
  }
}

void spinButtonPress() {
  spinButtonPressedFlag = true;
}

//IR SENSOR METHOD: UPDATE CREDIT & DISPLAY LCD MESSAGE
void updateCredit(int addAmt){                  //Method: Update Credit Amount
  if(addAmt>0 && creditAmt == 0){               //Scenario 1: Coin inserted when current credit == 0
    betAmt = 1;
    creditAmt += addAmt;
    totalCoinsInside += addAmt;
    coinInsert = !coinInsert;
  }else if(addAmt>0){                           //Scenario 2: Coin inserted when current credit !=0
    creditAmt += addAmt;
    totalCoinsInside += addAmt;
    coinInsert = !coinInsert;
  }else{                                        //Scenario 3: Credit deducted when played
    betAmt = 1;
    creditAmt += addAmt;
  }

}

//BET BTN METHOD: UPDATE BET AMOUNT
void updateBet(){                               //Method: Update Bet Amount when Bet Button Pressed
  Serial.println("updateBet() is called");
  if(betAmt>= maxBet || betAmt >= creditAmt){             //Scenario 1: Current bet amount equal to/greater than 3
    betAmt = 1;
  }else{                                                  //Scenario 2: Current bet amount less than 3
    betAmt++;
  }
}

//SPIN BTN METHOD: ACTIVATE LED MATRIX, UPDATE NECESSARY FIELDS AND DISPLAY
void activateSpin(){                                            //Method: Activate LED Matrix
  spinPressed = false;
  currentBetAmt = betAmt;                                           //Used to determine current bet amt
  updateCredit(betAmt*-1);                                              //Deducted credit based on bet amount
  //TODO: STARTING ANIMATION
  waterfall();
  activateLED();
}

void attachInterrupts() {
  attachInterrupt(digitalPinToInterrupt(betBtn), betButtonPress, FALLING);//BET BTTN
  attachInterrupt(digitalPinToInterrupt(spinBtn), spinButtonPress, FALLING);//Spin BTTN
}

void detachAllInterrupts(){
  detachInterrupt(digitalPinToInterrupt(betBtn));
  detachInterrupt(digitalPinToInterrupt(spinBtn));
}

//LCD DISPALY METHOD: PRINT MESSAGE BASED ON SITUATION
void LcdMessage(int scenario){
  lcd.clear();                                                    //Clear current displayed frame
  switch(scenario){
    //Idle State || Credit 0 Message Frame
    case 0 :  lcd.setCursor(7,1);
              lcd.print("INSERT");
              lcd.setCursor(8,2);
              lcd.print("COIN");
              break;

    //Credit!=0 && 0<Bet<=3 Message Frame
    case 1:   Serial.println("LCD Message 1 is called");
              lcd.setCursor(1,1);
              lcd.print("Credit : ");
              lcd.print(creditAmt);
              lcd.setCursor(1,2);
              lcd.print("Bet    : ");
              lcd.print(betAmt);
              break;

    //Lose Condition Message Frame    - "NBS", "SCE", "ADM"
    case 2:   lcd.setCursor(6,1);
              lcd.print("NICE TRY");
              lcd.setCursor(9,2);
              lcd.print(":)");
              break;

    //Win Condition Message Frame     - "EEE"
    case 3:   lcd.setCursor(6,1);
              lcd.print("YOU WON!");
              lcd.setCursor(9,2);
              lcd.print("$");
              lcd.print(currentBetAmt*2);
              break;

    //Jackpot Condition Message Frame - "IEM"
    case 4:   lcd.setCursor(6,1);
              lcd.print("JACKPOT!");
              lcd.setCursor(9,2);
              lcd.print(":)");
              break;

    case 5:   lcd.setCursor(4,1);
              lcd.print("TEMPORARILY");
              lcd.setCursor(3,2);
              lcd.print("OUT OF SERVICE");
              break;

    case 6:   lcd.setCursor(2,1);
              lcd.print("APPROACH COUNTER");
              lcd.setCursor(4,2);
              lcd.print("CREDIT : ");
              lcd.print(creditAmt);
              break;

    case 7:   lcd.setCursor(6,1);
              lcd.print("SPINNING");
              lcd.setCursor(8,2);
              lcd.print("...");
              break;

    //Default Message Frame
    default:  lcd.setCursor(1,1);
              lcd.print("Credit : ");
              lcd.print(creditAmt);
              lcd.setCursor(1,2);
              lcd.print("Bet : ");
              lcd.print(betAmt);
              break;
  }
}

//LED MATRIX METHOD: ACTIVATE LED MATRIX
void activateLED(){
  startingFrame = random(300) % numOfFrames; //choose random start frame
  endingFrame = random(300) % numOfFrames;   //choose random end frame

  displayStartingFrame(startingFrame);
  delay(2000);
  playAnimation(startingFrame, endingFrame, numberOfRotations);
}

//LED MATRIX METHOD: DISPLAY INITIAL/START FRAME
void displayStartingFrame(int startingFrame){
  int xPosCurrent[] = {1, 24, 47};
  int yPosCurrent[3];
  int currentCharacter[3];

  int ySpeed = 7;
  int yPosCenter = 6;

  int musicCounter = 0;

  uint16_t colour = redColor; //red

  //INITIALISE
  for (int i = 0; i < 3; i++){
    currentCharacter[i] = startingFrame; //initial char to display
    yPosCurrent[i] = -21;                //each char begin at top of matrix
  }

  //for each Character
  //bring it down to the center
  bool charArrivedAtCenter[] = {false, false, false};
  for (int i = 0; i < 3; i++){
    //extract character from character pointer
    char characterToPrint = extractCharFromFrameList(currentCharacter[i], i); //row, col respectively
    //drawCharacter(xPosCurrent[i], yPosCurrent[i], characterToPrint, colour);

    while (true){
      //DRAW FRAME
      matrix.fillScreen(blackColor); //FILL SCREEN 'black'
      drawCharacter(xPosCurrent[i], yPosCurrent[i], characterToPrint, colour);

      //draw all char that reached center again
      for (int j = 0; j < 3; j++){
        if (charArrivedAtCenter[j] == true){
          //draw character
          char charToPrint = extractCharFromFrameList(currentCharacter[j], j);
          drawCharacter(xPosCurrent[j], yPosCurrent[j], charToPrint, colour);
        }
      }

      //MOVE CHARACTERS
      yPosCurrent[i] += ySpeed;
      if (yPosCurrent[i] >= yPosCenter){
        charArrivedAtCenter[i] = true;
        yPosCurrent[i] = 6;
        //PLAY MUSIC
        tone(buzzer, fallingNotesMusic[musicCounter], 70);
        musicCounter = (musicCounter+1)%8;
        break;
      }
      matrix.swapBuffers(false);
    }
  }
  //draw all characters again
  matrix.fillScreen(blackColor); //FILL SCREEN 'black'
  for (int j = 0; j < 3; j++){
    //draw character
    char charToPrint = extractCharFromFrameList(currentCharacter[j], j);
    drawCharacter(xPosCurrent[j], yPosCurrent[j], charToPrint, colour);
  }

  matrix.swapBuffers(false);
}

//LED MATRIX METHOD: INITIALISE ANIMATION
void playAnimation(int startingFrame, int endingFrame, int numberOfRotations){
  // currentCharacter[] rationale explained:
  // This array stores 3 pointers that points to each character in a frame.
  // This pointers keep track of what character to display according to the following data structure:
  // Define a frame to be "EEE", or "SCE". These frames are stored in the array combo[]
  // Eg. combo[] contains {"EEE", "SCE", "IEM"...}
  // Index 0 keeps track of the 1st character in frame, index 1 keeps track of 2nd character, index 2 keeps track of 3rd character
  // Eg. for the frame "SCE" at combo[1],
  // currentCharacter[0] = 1 points at 'S', currentCharacter[1] = 1: 'C',currentCharacter[2] = 1: 'E'.
  // For the frame "EEE" and combo[0],
  // currentCharacter[0] = 0: 'E', currentCharacter[1] = 0: 'E', currentCharacter[2] = 0: 'E'
  int currentCharacter[] = {0, 0, 0};

  int currentXPositions[] = {1, 24, 47}; //xPos for character 1, 2 & 3 respectively
  int currentYPositions[] = {0, 0, 0};   //yPos for character 1, 2 & 3 respectively

  int musicCounter = 0;
  int numberOfIterations = 0;
  int startXPos = 1;
  int yPosCenter = 6;
  int yPosTop = -21; //yPos where character is outside of matrix

  Serial.println("playAnimation() called");
  Serial.print("spinButtonPressedFlag = ");
  Serial.println(spinButtonPressedFlag);

  //detach interrupt
  detachAllInterrupts();

  // initialise initial conditions
  for (int i = 0; i < 3; i++){
    // initial yPos of each character of each cylinder = center of matrix
    currentYPositions[i] = yPosCenter;
    //initial char to display
    currentCharacter[i] = startingFrame;
    currentYSpeeds[i] = ySpeedsConstant[i];
  }

  //begin animation...is rolling...for numberOfRotations times
  int slowestMovingCharIndex;
  for (int counter = 0; counter < numberOfRotations;){

    slowestMovingCharIndex = minimum(currentYSpeeds, 3);

    //DRAW CHARACTERS
    matrix.fillScreen(blackColor); //FILL SCREEN 'black'
    //for cylinder/character 1, 2, 3 respectively, draw character
    for (int i = 0; i < 3; i++){
      //extract character from character pointer
      char characterToPrint = extractCharFromFrameList(currentCharacter[i], i); //row, col respectively
      drawCharacter(currentXPositions[i], currentYPositions[i], characterToPrint, redColor);
    }
    matrix.swapBuffers(false);

    //UPDATE COORDINATES
    //for each character in cylinder 1, 2, 3, move yCoordinate of character by its respective ySpeed
    for (int i = 0; i < 3; i++){
      //move character down by scrollSpeed scaled by weights
      currentYPositions[i] += round(currentYSpeeds[i] * spdWeights[counter]);
      //check if current character has exited matrix
      if (currentYPositions[i] >= matrix.height() + 21){
        currentYPositions[i] = yPosTop; //wrap character around/reset to start Position

        //go to next character in column/cylinder
        currentCharacter[i] = (currentCharacter[i] + 1) % numOfFrames;

        if (slowestMovingCharIndex == i){ //counter value limited by slowest moving character
          counter++;
        }
      }
    }
    numberOfIterations++;
    if(numberOfIterations%2 == 0) {
      tone(buzzer, rollingMusic[musicCounter], 300);
      musicCounter = (musicCounter+1)%8;
    }
  }

  //PRINT ENDING FRAME
  for (int i = 0; i < 3; i++){
    currentCharacter[i] = endingFrame; //set each char pointer to point at endingFrame
  }

  //ending animation: draw ending frame
  //the following code aims to pull character to the center of the matrix
  //idea: frameArrivedAtCenter[] keeps track of whether the characters that has reached middle of matrix
  //if it has reached the center, oscillate about the center until all 3 characters reached the center
  //this code will be significant if speed diff is huge for each character
  bool frameArrivedAtCenter[] = {false, false, false}; //for cylinders 1, 2, 3 respectively
  uint16_t colour;
  while (true){
    matrix.fillScreen(blackColor);
    for (int i = 0; i < 3; i++){ //for each character of each cylinder,
      //DRAW CHARACTERS
      char characterToPrint = extractCharFromFrameList(currentCharacter[i], i); //row, col respectively
      drawCharacter(currentXPositions[i], currentYPositions[i], characterToPrint, colour);

      //UPDATE COORDINATES
      if (currentYPositions[i] <= yPosCenter){ //if character hasn't reached middle of matrix
        currentYPositions[i] += currentYSpeeds[i]; //move character down by its respective speed in ySpeed[]
      }else{ //this character has arrived/passed at/the center. Proceed to oscillate about center
        tone(buzzer, 1024, 200);
        //update character status: char has reached or passed the center
        frameArrivedAtCenter[i] = true;
        if (frameArrivedAtCenter[i] == true){
          currentYPositions[i] = 4; //force to (matrix center - 2) position. Position hardcoded, need to draw to visualise.
          currentYSpeeds[i] = 2;    //for oscillating logic. Char will osscilate about center of matrix w amplitude = 2 (+/- 2 about center)
        }
        currentYPositions[i] -= currentYSpeeds[i];
      }

      //CHOOSE COLOUR OF CHARACTERS TO PRINT
      if (frameArrivedAtCenter[i] == true){ //if char has passed the center, print winning condition colour
        colour = getColourToPrintBasedOnEndingFrame(endingFrame);
      }else{ //else, use original colour
        colour = redColor;
      }
    }
    matrix.swapBuffers(false);

    if (frameArrivedAtCenter[0] && frameArrivedAtCenter[1] && frameArrivedAtCenter[2]){ //if all frames has arrived at the center
      break;                                                                           //exit
    }
  }

  //final jitter animation to bring frames to a stop
  oscillateWithDecreasingEnergyAnimation(currentXPositions, currentYPositions, currentCharacter, endingFrame);
  //reattach interrupts()
  attachInterrupts();

  machineUpdates(endingFrame);
}

//LED MATRIX METHOD: TO GET INDEX OF MIN VALUE IN ARRAY METHOD
int minimum(float array[], int size){ //return index of min value in array
  float min = array[0];
  int index;
  for (int i = 1; i < size; i++){
    if (min > array[i]){
      min = array[i];
      index = i;
    }
  }
  return index;
}

//LED MATRIX METHOD: SET COLOR BASED ON COMBINATIONS
uint16_t getColourToPrintBasedOnEndingFrame(int endingFrame){
  String frameToPrint = combo[endingFrame];

  if (frameToPrint.equalsIgnoreCase("IEM")){ //JACKPOT
    return blueColor;
  }else if (frameToPrint.equalsIgnoreCase("EEE")){ //EEE
    return greenColor;
  }else{ //OTHERS
    return greenColor;
  }
}

//LED MATRIX METHOD: OSCILLATION ANIMATION ALGORITHMN
void oscillateWithDecreasingEnergyAnimation(int currentXPositions[], int currentYPositions[], int currentCharacter[], int endingFrame){ //(int endingFrame, int currentXPos, int[] currentYPositions)
  //String frameToPrint = combo[endingFrame];
  uint16_t colour = getColourToPrintBasedOnEndingFrame(endingFrame);

  //oscillateAnimation
  int yPositionsOscillate[] = {8, 4, 8, 4, 8, 6};
  for (int j = 0; j < 6; j++){
    matrix.fillScreen(blackColor); //FILL SCREEN 'black'
    for (int i = 0; i < 3; i++){ //for each character in cylinder 1, 2, 3 respectively
      //drawFrame
      char charToDraw = extractCharFromFrameList(currentCharacter[i], i);
      drawCharacter(currentXPositions[i], yPositionsOscillate[j], charToDraw, colour);
      tone(buzzer, 1024, 200);
    }
    matrix.swapBuffers(false);
  }
  attachInterrupts();
}

// LED MATRIX METHOD: EXTRACT CHAT FROM COMBO
char extractCharFromFrameList(int rowNumber, int colNumber){
  return combo[rowNumber].charAt(colNumber); //eg. combo[1] returns "NBS". combo[1].at(2) returns "B".
}

//LED MATRIX METHOD: DRAW CHARACTER, SET COLOR & POSITION
void drawCharacter(int xPos, int yPos, char characterToPrint, uint16_t color){
  // DRAW Text
  uint8_t w = 0;
  matrix.setTextColor(color);
  matrix.setCursor(xPos, yPos);
  matrix.print(characterToPrint);
}

//SERVER MOTOR METHOD: TO DISPENSE COIN
void dispenseCoin(int amount){
  DispenseCoinsSFX();
  myservo.attach(MotorPin);
  for(int i = 0; i < amount; i++){
    myservo.write(servoStartAngle);
    delay(230);
    myservo.write(servoEndAngle);
    delay(230);
  }
  totalCoinsInside -= amount;
  myservo.detach();
}

bool AdminCoinInsert(){
  if(adminCoin <3){
    if(isObstacle == LOW) {                       // Obstacle detected
      coinInsert = true;
    } else if(isObstacle == HIGH && coinInsert){  // No Obstacle AND coinInsert == True
      adminCoin += 1;
      totalCoinsInside += adminCoin;
    }
    return false;
  }else{
    return true;
  }
}

//LCD, SERVER MOTOR METHOD: NECESSARY ACTIONS TAKEN BASED ON CONDITION
void machineUpdates(int endFrameIndex){
  Serial.println("machineUpdate() called");
  String endingFrame = combo[endFrameIndex];
  if(endingFrame.equalsIgnoreCase(Jackpot)){
    Serial.println("Jackpot");
    LcdMessage(4);
    JackpotSFX();

    dispenseCoin(totalCoinsInside);
    creditAmt -= creditAmt;

    //TODO: JACKPOT LED ANIMATION
    firework();
    LcdMessage(5);
    detachAllInterrupts(); //do not allow user to insert coin anymore
  }else if(endingFrame.equalsIgnoreCase(Win)){
    Serial.println("Win");
    LcdMessage(3);
    WinConditionSFX();

    //TODO: WIN LED ANIMATION
    radiation();
    unsigned long time = millis();
    dispenseCoin(currentBetAmt*winRate);
    Serial.print("Time spent: ");
    Serial.print(millis() - time);

    if(totalCoinsInside < winRate){
      LcdMessage(5);
    }else if(creditAmt <= 0){                             //Message Type 1: Credit less than or equal to 0
      LcdMessage(0);
    }else{                                          //Message Type 2: When credit greater than 0
      LcdMessage(1);
    }
  }else{
    Serial.println("Lose");
    LcdMessage(2);
    LoseSFX();

    //TODO: LOSE LED ANIMATION
    sadFace();
    delay(2000);
    if(totalCoinsInside < winRate){
      LcdMessage(5);
    }else if(creditAmt <= 0){                             //Message Type 1: Credit less than or equal to 0
      LcdMessage(0);
    }else{                                          //Message Type 2: When credit greater than 0
      LcdMessage(1);
    }
  }
}
void StartUpSFX() {
  tone(buzzer, 260);
  delay(250);
  tone(buzzer, 330);
  delay(120);
  tone(buzzer, 349);
  delay(120);
  tone(buzzer, 392);
  delay(250);
  tone(buzzer, 490);
  delay(250);
  tone(buzzer, 523);
  delay(200);
  noTone(buzzer);
  // delay(2000);
}

void CoinInsertSFX() {
  tone(buzzer, 1000);
  delay(70);
  tone(buzzer, 1500);
  delay(70);
  noTone(buzzer);
  // delay(2000);
}

void BetIncrementSFX() {
  tone(buzzer, 1319);
  delay(90);
  tone(buzzer, 1047);
  delay(90);
  tone(buzzer, 1319);
  delay(90);
  tone(buzzer, 1047);
  delay(200);
  noTone(buzzer);
  // delay(2000);
}

void SpinActivateSFX() {
  for (int count = 0; count < 8; count++) {
    tone(buzzer, 400);
    delay(70);
    tone(buzzer, 600);
    delay(70);
  }
  noTone(buzzer);
  // delay(2000);
}

void LoseSFX() {
  tone(buzzer, 1500);
  delay(200);
  tone(buzzer, 1250);
  delay(200);
  tone(buzzer, 1000);
  delay(200);
  tone(buzzer, 1100);
  delay(250);
  noTone(buzzer);
  // delay(2000);
}

void WinConditionSFX() {
  tone(buzzer, 900);
  delay(80);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 900);
  delay(250);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 900);
  delay(100);
  tone(buzzer, 1150);
  delay(100);
  tone(buzzer, 1350);
  delay(250);
  tone(buzzer, 1150);
  delay(100);
  tone(buzzer, 1350);
  delay(300);
  noTone(buzzer);
  // delay(2000);
}

void JackpotSFX() {
  tone(buzzer, 196);
  delay(150);
  tone(buzzer, 262);
  delay(150);
  tone(buzzer, 330);
  delay(150);
  tone(buzzer, 392);
  delay(150);
  tone(buzzer, 523);
  delay(150);
  tone(buzzer, 659);
  delay(150);
  tone(buzzer, 784);
  delay(450);
  tone(buzzer, 659);
  delay(450);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 116);
  delay(150);
  tone(buzzer, 294);
  delay(150);
  tone(buzzer, 311);
  delay(150);
  tone(buzzer, 415);
  delay(150);
  tone(buzzer, 523);
  delay(150);
  tone(buzzer, 621);
  delay(150);
  tone(buzzer, 830);
  delay(450);
  tone(buzzer, 621);
  delay(450);

  noTone(buzzer);
  delay(50);

  tone(buzzer, 207);
  delay(150);
  tone(buzzer, 262);
  delay(150);
  tone(buzzer, 349);
  delay(150);
  tone(buzzer, 233);
  delay(150);
  tone(buzzer, 587);
  delay(150);
  tone(buzzer, 698);
  delay(150);
  tone(buzzer, 932);
  delay(400);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 932);
  delay(150);
  noTone(buzzer);
  delay(10);
  tone(buzzer, 932);
  delay(150);
  noTone(buzzer);
  delay(10);
  tone(buzzer, 932);
  delay(150);
  noTone(buzzer);
  delay(10);
  tone(buzzer, 1047);
  delay(550);
  noTone(buzzer);
  // delay(2000);
}

void DispenseCoinsSFX() {
  for (int count = 0; count < 7; count++) {     //set limit of count to number of coins dispensing?
    tone(buzzer, 800);
    delay(90);
    tone(buzzer, 1000);
    delay(90);
  }

  tone(buzzer, 1600);
  delay(100);
  noTone(buzzer);
  // delay(2000);
}

void drawMessage(bool isWinning){
  detachAllInterrupts();
  Serial.println("Winning message is displayed");
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(12,12);
  matrix.setTextSize(1);
  if (isWinning){
    matrix.print("YOU WON");
  }else{
    matrix.print("NICE TRY :)");
  }
  matrix.swapBuffers(false);
  matrix.setTextSize(3);

  //SFX
  tone(buzzer, 1024, 100);
  delay(200);
  tone(buzzer, 1024, 200);
  delay(1000);
  attachInterrupts();
}

// ANIMATION 1: WATERFALL
void waterfall(){
  detachAllInterrupts();

  // Generate waterfall columns             This gives the X0, X1
  for (int i = 0; i < 16; i++)  {
    waterfallColumns[i] = (4 * i + 1);
    Serial.println(waterfallColumns[i]);
  }
  // Generate waterfall starting positions  This gives the Y0
  for (int i = 0; i < 16; i++)  {
    waterfallStartingPos[i] = random(0, 31);
  }
  // Rolling animation
  for (int i = 0; i < waterfallRotations; i += 3)  {
    matrix.fillScreen(blackColor);
    for (int j = 1; j < 8; j++)    {
      // Get the starting and ending pos
      int yStart1 = waterfallStartingPos[15 + j + 1] + i;
      int yStart2 = waterfallStartingPos[15 - j] + i;
      int yStart3 = waterfallStartingPos[j] + i;
      int yStart4 = waterfallStartingPos[23 + j + 1];
      int yEnd1 = yStart1 - 32;
      int yEnd2 = yStart2 - 32;
      int yEnd3 = yStart3 - 32;
      int yEnd4 = yStart4 - 32;
      drawWaterfall(yStart1, yEnd1, 15+j+1);
      drawWaterfall(yStart2, yEnd2, 15-j);
      drawWaterfall(yStart3, yEnd3, j);
      drawWaterfall(yStart4, yEnd4, 23+j+1);
    }
    matrix.swapBuffers(false);
    delayMicroseconds(120000);
  }
  attachInterrupts();
}
void drawWaterfall(int yStart, int yEnd, int index){
  if (yEnd < 0 and yStart < 31){
    yEnd = 0;
  }else if (yStart > 31){
    yStart = 31;
  }else if (yEnd > 31){
    yStart = 0;
    yEnd =0;
  }
  // Changing color
  int section1 = 12 + sin(index);
  int section2 = 24 + sin(index);
  if (yStart < section1){
    matrix.drawLine(waterfallColumns[index], yStart, waterfallColumns[index], yEnd, waterfallColor1);
  }else if (yStart < section2){
    matrix.drawLine(waterfallColumns[index], section1 - 1, waterfallColumns[index], yEnd, waterfallColor1);
    matrix.drawLine(waterfallColumns[index], yStart, waterfallColumns[index], section1, waterfallColor2);
  }else{
    if (yEnd < section1){
      matrix.drawLine(waterfallColumns[index], section1 - 1, waterfallColumns[index], yEnd, waterfallColor1);
      matrix.drawLine(waterfallColumns[index], section2 - 1, waterfallColumns[index], section1, waterfallColor2);
      matrix.drawLine(waterfallColumns[index], yStart, waterfallColumns[index], section2, waterfallColor3);
    }else if (yEnd < section2){
      matrix.drawLine(waterfallColumns[index], section2 - 1, waterfallColumns[index], yEnd, waterfallColor2);
      matrix.drawLine(waterfallColumns[index], yStart, waterfallColumns[index], section2, waterfallColor3);
    }else{
      matrix.drawLine(waterfallColumns[index], yStart, waterfallColumns[index], yEnd, waterfallColor3);
    }
  }
}

// ANIMATION 2: RADIATION
void radiation(){
  detachAllInterrupts();
  matrix.fillScreen(matrix.Color333(0,0,0));

  for (int i = 0; i < radiationRotations; i++){
    // section 1 circle
    for (int r = 1; r < 6; r++){
      matrix.drawCircle(31, 15, r, Wheel((i+r+10)%24));
    }

    // section 2 circle
    for (int r = 6; r < 15; r++){
      matrix.drawCircle(31, 15, r, Wheel((i+r+10)%24));
      // matrix.drawCircle(31, 15, r, radiationColors[(1 + i) % 4]);
    }

    // section 3 circle
    for (int r = 15; r < 24; r++){
      matrix.drawCircle(31, 15, r, Wheel((i+r+10)%24));
      // matrix.drawCircle(31, 15, r, radiationColors[(2 + i) % 4]);
    }

    // section 4 circle
    for (int r = 24; r < 32; r++){
      matrix.drawCircle(31, 15, r, Wheel((i+r+10)%24));
      // matrix.drawCircle(31, 15, r, radiationColors[(3 + i) % 4]);
    }
    matrix.swapBuffers(false);
  }
  drawMessage(true);
  attachInterrupts();
}

// ANIMATION 3: TRIANGLE SPINNING
void triangleSpinning(){
  int centerX = 31;
  int centerY = 15;
  for (int i = 0; i < triangleNumberofRotations; i++){
    for (int angle = 0; angle < 360; angle += 5){
      matrix.fillScreen(blackColor);
      int x1 = centerX + triangleRadius * cos(angle * (pi / 180));
      int y1 = centerY + triangleRadius * sin(angle * (pi / 180));

      int x2 = centerX + triangleRadius * cos((angle + 120) * (pi / 180));
      int y2 = centerY + triangleRadius * sin((angle + 120) * (pi / 180));

      int x3 = centerX + triangleRadius * cos((angle + 240) * (pi / 180));
      int y3 = centerY + triangleRadius * sin((angle + 240) * (pi / 180));

      uint16_t color;
      if (triangleNumberofRotations < 3){
        color = triangleColors[0];
      }else if (triangleNumberofRotations < 6){
        color = triangleColors[1];
      }else{
        color = triangleColors[2];
      }
      matrix.drawLine(x1, y1, x2, y2, color);
      matrix.drawLine(x2, y2, x3, y3, color);
      matrix.drawLine(x3, y3, x1, y1, color);
      matrix.swapBuffers(false);
      delayMicroseconds(5000);
    }
  }
}

// ANIMATION 4: FAIL SAD FACE
void sadFace(){
  detachAllInterrupts();
  matrix.fillScreen(blackColor);
  drawFace(0 + 10, 0 + 10);
  tone(buzzer, 523, 500);
  delay(500);//delayMicroseconds(1000000);

  matrix.fillScreen(blackColor);
  drawFace(31 - 10, 31 - 10);
  tone(buzzer, 494,500);
  delay(500);//delayMicroseconds(1000000);


  matrix.fillScreen(blackColor);
  drawFace(31 + 10, 0 + 10);
  tone(buzzer, 466, 500);
  delay(500);//delayMicroseconds(1000000);

  matrix.fillScreen(blackColor);
  drawFace(63 - 10, 31 - 10);
  tone(buzzer, 440,500);
  delay(2000);  //delayMicroseconds(1000000);

  drawMessage(false);
  attachInterrupts();
}

void drawFace(int x, int y){
  uint16_t faceColor = matrix.Color333(123, 123, 123);
  matrix.drawCircle(x, y, 10, faceColor);        // Face outline
  matrix.fillCircle(x - 3, y - 3, 2, faceColor); // Left eye
  matrix.fillCircle(x + 3, y - 3, 2, faceColor); // Right eye
  int centerX = x;
  int centerY = y+7;
  int mouthRadius = 4;
  for (int angle = 0; angle > -180; angle -= 5){ // Mouth
    int i = centerX + mouthRadius * cos(angle * (pi / 180));
    int j = centerY + mouthRadius * sin(angle * (pi / 180));
    matrix.drawPixel(i, j, faceColor);
  }
  matrix.swapBuffers(false);
}

// ANIMATION 5: FIREWORKS
void firework(){
  musicCounter = 0;
  detachAllInterrupts();
  matrix.fillScreen(blackColor);
    if(roll){
        drawFirework( random(set1XStart, set1XEnd), random(yMin, yMax), matrix.Color333(7, 0, 0), matrix.Color333(3, 0, 0), 20);//10);
        drawFirework( random(set3XStart, set3XEnd), random(yMin, yMax), matrix.Color333(0, 7, 0), matrix.Color333(0, 3, 0), 20);
        drawFirework( random(set2XStart, set2XEnd), random(yMin, yMax), matrix.Color333(0, 0, 7), matrix.Color333(0, 0, 3), 20);//15);

        drawFirework( random(set1XStart, set1XEnd), random(yMin, yMax), matrix.Color333(7, 3, 1), matrix.Color333(3, 1, 0), 20);
        drawFirework( random(set3XStart, set3XEnd), random(yMin, yMax), matrix.Color333(7, 7, 7), matrix.Color333(3, 1, 0), 20);//16);
        drawFirework( random(set2XStart, set2XEnd), random(yMin, yMax), matrix.Color333(3, 0, 7), matrix.Color333(1, 0, 3), 20);//18);

        drawFirework( random(set1XStart, set1XEnd), random(yMin, yMax), matrix.Color333(5, 3, 0), matrix.Color333(7, 1, 1), 20);
        drawFirework( random(set3XStart, set3XEnd), random(yMin, yMax), matrix.Color333(7, 0, 0), matrix.Color333(7, 3, 0), 20);//16);
        drawFirework( random(set2XStart, set2XEnd), random(yMin, yMax), matrix.Color333(3, 7, 7), matrix.Color333(7, 3, 3), 20);//18);
        roll =false;
    }
    drawMessage(true);
    attachInterrupts();
}

void drawFirework(byte x, byte y, uint16_t lineColor, uint16_t radColor, uint8_t delayTime) {

  for( byte i=32; i>y; i--) {
    matrix.drawLine(x, i, x, (i+1), lineColor);
    matrix.swapBuffers(true);
    delayMicroseconds(delayTime*1000);
    matrix.drawLine(x, i, x, (i+1), blackColor);
    matrix.swapBuffers(true);
    
    if(i%4==0) {
      //PLAY MUSIC
      tone(buzzer, fireworkMusic[musicCounter], 200);
      musicCounter = (musicCounter+1)%29;
    }
  }
  delayMicroseconds(delayTime*1000);
  matrix.drawCircle(x, y, 1, lineColor); delayMicroseconds(delayTime*3000);;
  matrix.drawCircle(x, y, 1, blackColor);

  for ( byte j=1;j<4; j++) {
    matrix.drawLine(x, (y-5)-j, x, (y-4)-j, lineColor);
    matrix.swapBuffers(true);
    matrix.drawLine(x, (y+2)+j, x, (y+3)+j, lineColor);
    matrix.swapBuffers(true);
    
    matrix.drawLine((x-5)-j, y, (x-4)-j, y, lineColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x+2)+j, y, (x+3)+j, y, lineColor);
    matrix.swapBuffers(true);

    matrix.drawLine((x+1)+j, (y+1)+j, (x+3)+j, (y+3)+j, radColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-1)-j, (y+1)+j, (x-3)-j, (y+3)+j, radColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x+1)+j, (y-1)-j, (x+3)+j, (y-3)-j, radColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-1)-j, (y-1)-j, (x-3)-j, (y-3)-j, radColor);
    matrix.swapBuffers(true);

    delayMicroseconds(delayTime*2000);
    //PLAY MUSIC
    tone(buzzer, fireworkMusic[musicCounter], 100);
    musicCounter = (musicCounter+1)%29;
    

    matrix.drawLine(x, (y-5)-(j-1), x, (y-4)-(j-1), blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine(x, (y+2)+(j-1), x, (y+3)+(j-1), blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-5)-(j-1), y, (x-4)-(j-1), y, blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x+2)+(j-1), y, (x+3)+(j-1), y, blackColor);
    matrix.swapBuffers(true);

    matrix.drawLine((x+1)+(j-1), (y+1)+(j-1), (x+3)+(j-1), (y+3)+(j-1), blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-1)-(j-1), (y+1)+(j-1), (x-3)-(j-1), (y+3)+(j-1), blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x+1)+(j-1), (y-1)-(j-1), (x+3)+(j-1), (y-3)-(j-1), blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-1)-(j-1), (y-1)-(j-1), (x-3)-(j-1), (y-3)-(j-1), blackColor);
    matrix.swapBuffers(true);

    delayMicroseconds(delayTime*2000);

    matrix.drawLine(x, (y-5)-j, x, (y-4)-j, blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine(x, (y+2)+j, x, (y+3)+j, blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-5)-j, y, (x-4)-j, y, blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x+2)+j, y, (x+3)+j, y, blackColor);
    matrix.swapBuffers(true);

    matrix.drawLine((x+1)+j, (y+1)+j, (x+3)+j, (y+3)+j, blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-1)-j, (y+1)+j, (x-3)-j, (y+3)+j, blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x+1)+j, (y-1)-j, (x+3)+j, (y-3)-j, blackColor);
    matrix.swapBuffers(true);
    matrix.drawLine((x-1)-j, (y-1)-j, (x-3)-j, (y-3)-j, blackColor);
    matrix.swapBuffers(true);

    //PLAY MUSIC
    tone(buzzer, fireworkMusic[musicCounter], 100);
    musicCounter = (musicCounter+1)%29;
  }
}

void rocketAnimation(){
     for(int index = -2; index < 132; index +=6 ){
    //int index =64;
    //Clear LED Screen
    matrix.fillScreen(blackColor);

    //Draw Big Rocket
    matrix.fillTriangle(mainRck_RT_X1+index, mainRck_RT_Y1, mainRck_RT_X2+index, mainRck_RT_Y2, mainRck_RT_X1+index, mainRck_RT_Y3, orangeColor);
    matrix.fillTriangle(mainRck_CenTri_X1+index, mainRck_TT_Y1, mainRck_CenTri_X2+index, mainRck_TT_Y2, mainRck_CenTri_X1+index, mainRck_TT_Y2, orangeColor);
    matrix.fillTriangle(mainRck_CenTri_X1+index, mainRck_BT_Y1, mainRck_CenTri_X2+index, mainRck_BT_Y1, mainRck_CenTri_X1+index, mainRck_BT_Y2, orangeColor);

    matrix.fillRect(mainRck_Rect_X+index, mainRck_Rect_Y, mainRck_Rect_W, mainRck_Rect_H, blueColor);

    matrix.drawLine(mainRck_EL_Start+index, mainRck_EL_R1R, mainRck_EL_Start+mainRck_EL_Length+index,mainRck_EL_R1R, redColor);
    matrix.drawLine(mainRck_ES_Start+index, mainRck_ES_R2R, mainRck_ES_Start+mainRck_ES_Length+index,mainRck_ES_R2R, redColor);
    matrix.drawLine(mainRck_EL_Start+index, mainRck_EL_R3R, mainRck_EL_Start+mainRck_EL_Length+index,mainRck_EL_R3R, redColor);
    matrix.drawLine(mainRck_ES_Start+index, mainRck_ES_R4R, mainRck_ES_Start+mainRck_ES_Length+index,mainRck_ES_R4R, redColor);
    matrix.drawLine(mainRck_EL_Start+index, mainRck_EL_R5R, mainRck_EL_Start+mainRck_EL_Length+index,mainRck_EL_R5R, redColor);

    //Draw Char
    matrix.drawChar(firstChar_X+index, Char_Y, 'I', blueColor, blackColor, 2);
    matrix.drawChar(secondChar_X+index, Char_Y, 'E', blueColor, blackColor, 2);
    matrix.drawChar(thirdChar_X+index, Char_Y, 'M', blueColor, blackColor, 2);


    //Draw Small Rockets
    matrix.fillTriangle(leftRck_RT_X1+index, leftRck_RT_Y1, leftRck_RT_X2+index, leftRck_RT_Y2, leftRck_RT_X1+index, leftRck_RT_Y3, orangeColor);
    matrix.fillTriangle(leftRck_RT_X1+index, leftRck_RT_Y4, leftRck_RT_X2+index, leftRck_RT_Y5, leftRck_RT_X1+index, leftRck_RT_Y6, orangeColor);

    matrix.fillRect(leftRect_X+index, leftRect_top_Y, leftRect_W, leftRect_H, blueColor);
    matrix.fillRect(leftRect_X+index, leftRect_bottom_Y, leftRect_W, leftRect_H, blueColor);

    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_TT1_Y1, leftRck_CenTri_X2+index, leftRck_TT1_Y2, leftRck_CenTri_X1+index, leftRck_TT1_Y2, orangeColor);
    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_TT2_Y1, leftRck_CenTri_X2+index, leftRck_TT2_Y2, leftRck_CenTri_X1+index, leftRck_TT2_Y2, orangeColor);

    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_BT1_Y1, leftRck_CenTri_X2+index, leftRck_BT1_Y1, leftRck_CenTri_X1+index, leftRck_BT1_Y2, orangeColor);
    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_BT2_Y1, leftRck_CenTri_X2+index, leftRck_BT2_Y1, leftRck_CenTri_X1+index, leftRck_BT2_Y2, orangeColor);

    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R1R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R1R ,redColor);
    matrix.drawLine(leftRck_ES_Start+index, leftRck_EL_R2R, leftRck_ES_Start+leftRck_ES_Length+index, leftRck_EL_R2R ,redColor);
    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R3R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R3R ,redColor);
    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R4R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R4R ,redColor);
    matrix.drawLine(leftRck_ES_Start+index, leftRck_EL_R5R, leftRck_ES_Start+leftRck_ES_Length+index, leftRck_EL_R5R ,redColor);
    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R6R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R6R ,redColor);
    matrix.swapBuffers(false);

    if(index == 64){
      StartUpSFX();
    }else{
      delayMicroseconds(5);
    }
  }
}

void drawLine(int xStart, int yStart, int length){
  for (int i = yStart; i < length; i++)  { //todo: length or length+1?
    uint16_t colour = Wheel((i + xStart) % 24);
    int currentYPos = i;
    matrix.drawPixel(xStart, currentYPos, colour);
  }
}
// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos){
  if (WheelPos < 8)  {
    return matrix.Color333(7 - WheelPos, WheelPos, 0);
  }else if (WheelPos < 16)  {
    WheelPos -= 8;
    return matrix.Color333(0, 7 - WheelPos, WheelPos);
  }else{
    WheelPos -= 16;
    return matrix.Color333(WheelPos, 0, 7 - WheelPos);
  }
}
