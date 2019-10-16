#include <Wire.h> 
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <RGBmatrixPanel.h>
#include <Buzzer.ino>

//Arduino Mega Pin Definition
#define A A0              // ROW SELECT: WHICH TWO ROWS CURRENTLY LIT
#define B A1
#define C A2
#define D A3

#define MotorPin 5
#define speakerPin 7
#define OE 9              // OUTPUT ENABLE
#define LAT 10            // LATCH SIGNAL MARKS THE END OF A ROW OF DATA
#define CLK 11            // USE THIS ON ARDUINO MEGA
#define betBtn 12
#define spinBtn 13
#define isObstaclePin 14
#define reservePin 15

#define R1 24             // UPPER RGB DATA - TOP HALF OF DISPLAY
#define G1 25
#define B1 26
#define R1 27             // LOWER RGB DATA - BOTTOM HALF OF DISPLAY
#define G2 28
#define B2 29

//Array of Combos & Select, Combinations of all possible Frames (10)
String combo[] = {"EEE", "NBS", "IEM", "ADM", "SCE", "NBS", "SCE", "ADM", "EEE", "SCE"};

//Array to determine scroll speed
//ySpeed, or scroll speed is proportional to weights (20)
float spdWeights[] = {0.6, 0.7, 0.8, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.4, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5};

//CONSTRUCTOR FOR 64x32 LED MATRIX PANEL
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

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
int creditAmt =0, betAmt=0, currentBetAmt=0;

//IR sensor variable
bool coinInsert = false;

//Server Motor Variable
Servo myservo;
const int minCoinsRequired = 3;
int totalCoinsInside = 3;
int intermediateSpinPos = 90;
int winRate = 2;

void setup()
{
  Serial.begin(9600);
  myservo.attach(MotorPin); 
  lcd.init();                                    // Initialize the LCD 
  pinMode(isObstaclePin, INPUT);
  pinMode(betBtn, INPUT);
  pinMode(spinBtn, INPUT);
  
  //Set-up code for matrix
  matrix.begin();
  //set text properties
  matrix.setTextSize(3);            // size 1 -> hxw = 7x4. size = 3 -> hxw = (7*3)x(4*3) = 21x12
  matrix.setTextWrap(false);        // Don't wrap at end of line - will do ourselves

  // *MUST SEED using Analog input from UNUSED_PIN. analogRead(reservePin).
  randomSeed(analogRead(reservePin));

  //LCD Display "Insert Coin" Message
  LcdMessage(0);
}

void loop()
{ 
  lcd.backlight();
  if(totalCoinsInside >= minCoinsRequired){
    isObstacle = digitalRead(isObstaclePin);
  
    if(isObstacle == LOW) {                       // Obstacle detected
      // TODO: IDLE STATE ANIMATION
      coinInsert = true;
    } else if(isObstacle == HIGH && coinInsert){  // No Obstacle AND coinInsert == True
      updateCredit(1);                            // Incremeent Credit Amt
      LcdMessage(1);                              // Display Message in LCD
    }
    
    if(betAvail()){                               //Check if bet increment could be done
      updateBet();                                    //Call method to update bet amt
    }else if(spinAvail()){                        //Ceck if spin could be done
      activateSpin();                                 //Call Method to start LED Matrix animation
    }
  }else{
    // TODO: OUT OF SERVICE ANIMATION
    LcdMessage(5);
  }
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
    creditAmt += addAmt;               
    if(creditAmt <= 0){                             //Message Type 1: Credit less than or equal to 0
      LcdMessage(0);
    }else{                                          //Message Type 2: When credit greater than 0
      LcdMessage(1);
    }
  }
}

//BET BTN METHOD: UPDATE BET AMOUNT
void updateBet(){                               //Method: Update Bet Amount when Bet Button Pressed
  if(betAmt>=3 || betAmt >= creditAmt){             //Scenario 1: Current bet amount equal to/greater than 3
    Serial.println("Bet = 1");
    Serial.println(betAmt);
    betAmt = 1;
  }else{                                            //Scenario 2: Current bet amount less than 3
    Serial.println("Increase Bet");
    Serial.println(betAmt);
    betAmt++;
  }
  betPressed = false;
  LcdMessage(1);
}

//BET BTN METHOD: CHECK IF INCREMENT COULD BE ALLOWED
bool betAvail(){
  betState = digitalRead(betBtn);                                   //Read Bet Btn Input 
  if(betState == HIGH){
    betPressed = true;
    return false;
  }else if(betState == LOW && creditAmt > 0 && betPressed){        //Bet Btn Pressed && credit Amt greater than 0   
    Serial.println("Bet to increase");
    return true;
  }else{
    betPressed = false;
    return false;
  }
}

//SPIN BTN METHOD: ACTIVATE LED MATRIX, UPDATE NECESSARY FIELDS AND DISPLAY
void activateSpin(){                                            //Method: Activate LED Matrix
  spinPressed = false;                                              
  currentBetAmt = betAmt;                                           //Used to determine current bet amt
  creditAmt -= betAmt;                                              //Deducted credit based on bet amount
  if(creditAmt <= 0){
    creditAmt = 0;                                                  //Credit amount set back to 0
    betAmt = 0;                                                     //Bet amount set back to 0
    LcdMessage(0);                                                  //Display "Insert Coin" Message when credit less than 0    
  }else{
    betAmt = 1;                                                     //Bet amount set to 1
    LcdMessage(1);                                                  //Display remaining credit and bet amounts in LCD
  }
  // TODO: STARTING ANIMATION
  activateLED();
}

//SPIN BTN METHOD: CHECK IF BTN COULD ACTIVATE LED TO SPIN 
bool spinAvail(){
  spinState = digitalRead(spinBtn);                               //Read  Spin Btn Input
  if(spinState == HIGH){
    spinPressed = true;
  }else if (spinState == LOW && creditAmt > 0 && spinPressed) {
    if(totalCoinsInside < (winRate * betAmt)){
      LcdMessage(6);
    }
    Serial.println("Spin");
    return true;
  }else{
    spinPressed = false;
    return false;
  }
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
    case 1:   lcd.setCursor(1,1);
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
              lcd.print(betAmt*2);
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

    case 6:   lcd.setCursor(3,1);
              lcd.print("MAXIMUM: ");
              lcd.print(totalCoinsInside);
              lcd.setCursor(2,2);
              lcd.print("YOU CAN WIN");
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
  playAnimation(startingFrame, endingFrame, numberOfRotations);
}

//LED MATRIX METHOD: DISPLAY INITIAL/START FRAME
void displayStartingFrame(int startingFrame){
  //initialise initial conditions:
  //lazy... hardcoded:
  int xPos[] = {1, 24, 47};
  int yPos[] = {6, 6, 6};
  int currentCharacter[] = {0, 0, 0};
  
  for (int i = 0; i < 3; i++){
    //initial char to display
    currentCharacter[i] = startingFrame;
  }
  
  //Draw Characters
  for (int i = 0; i < 3; i++){
    char charToDraw = extractCharFromFrameList(currentCharacter[i], i);
    drawCharacter(xPos[i], yPos[i], charToDraw, redColor);
  }
}

//LED MATRIX METHOD: INITIALISE ANIMATION 
void playAnimation(int startingFrame, int endingFrame, int numberOfRotations){
  /* currentCharacter[] stores 3 pointers that points to each character in a frame.
      This pointers keep track of what character to display according to the following data structure:
      Define a frame to be "EEE", or "SCE". These frames are stored in the array combo[]
      
      Eg. combo[] contains {"EEE", "SCE", "IEM"...}
      Index 0 keeps track of the 1st character in frame, index 1 keeps track of 2nd character, index 2 keeps track of 3rd character
      
      Eg. for the frame "SCE" at combo[1],
      currentCharacter[0] = 1 points at 'S', currentCharacter[1] = 1: 'C',currentCharacter[2] = 1: 'E'.
  
      For the frame "EEE" and combo[0],
      currentCharacter[0] = 0: 'E', currentCharacter[1] = 0: 'E', currentCharacter[2] = 0: 'E'
  */
  
  int currentCharacter[] = {0, 0, 0};
  int currentXPositions[] = {1, 24, 47};      //xPos for character 1, 2 & 3 respectively
  int currentYPositions[] = {0, 0, 0};        //yPos for character 1, 2 & 3 respectively

  int startXPos = 1;
  int yPosCenter = 6;
  int yPosTop = -21;                         //yPos where character is outside of matrix

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
    matrix.fillScreen(blackColor); //FILL SCREEN 'black'
    slowestMovingCharIndex = minimum(currentYSpeeds, 3);

    //DRAW CHARACTERS : By cylinder/character 1, 2, 3 respectively, draw character
    for (int i = 0; i < 3; i++){
      //extract character from character pointer
      char characterToPrint = extractCharFromFrameList(currentCharacter[i], i);                                     //row, col respectively
      drawCharacter(currentXPositions[i], currentYPositions[i], characterToPrint, redColor);
    }
    delay(0);

    //UPDATE COORDINATES
    //For each character in cylinder 1, 2, 3, move yCoordinate of character by its respective ySpeed
    for (int i = 0; i < 3; i++){
      //Move character by scrollSpeed scaled by weights
      currentYPositions[i] += round(currentYSpeeds[i] * spdWeights[counter]);
      
      //Check if current character has exited matrix
      if (currentYPositions[i] >= matrix.height() + 21) //text ht = 21
      {
        currentYPositions[i] = yPosTop; //wrap character around/reset to start Position

        //Go to next character in column/cylinder
        currentCharacter[i] = (currentCharacter[i] + 1) % numOfFrames;

        if (slowestMovingCharIndex == i){ //counter value limited by slowest moving character
          counter++;
        }
      }
    }
  }

  //PRINT ENDING FRAME
  for (int i = 0; i < 3; i++){
    currentCharacter[i] = endingFrame; //set each char pointer to point at endingFrame
  }

  /*ENDING ANIMATION: draw ending frame to pull character to the center of the matrix
    IDEA:     frameArrivedAtCenter[] keeps track of whether the characters that has reached middle of matrix
              if it has reached the center, oscillate about the center until all 3 characters reached the center
              this code will be significant if speed diff is huge for each character
  */
  
  bool frameArrivedAtCenter[] = {false, false, false}; //for cylinders 1, 2, 3 respectively
  uint16_t colour;
  while (true){
    matrix.fillScreen(blackColor);
    for (int i = 0; i < 3; i++){                 //for each character of each cylinder,
      //DRAW CHARACTERS
      char characterToPrint = extractCharFromFrameList(currentCharacter[i], i); //row, col respectively
      drawCharacter(currentXPositions[i], currentYPositions[i], characterToPrint, colour);

      //UPDATE COORDINATES
      if (currentYPositions[i] <= yPosCenter){                    //if character hasn't reached middle of matrix
        currentYPositions[i] += currentYSpeeds[i];                //move character down by its respective speed in ySpeed[]
      }else{                                                      //this character has arrived/passed at/the center. Proceed to oscillate about center
        //update character status: char has reached or passed the center
        frameArrivedAtCenter[i] = true;
        if (frameArrivedAtCenter[i] == true){
          currentYPositions[i] = 4;                               //force to (matrix center - 2) position. Position hardcoded, need to draw to visualise.
          currentYSpeeds[i] = 2;                                  //for oscillating logic. Char will osscilate about center of matrix w amplitude = 2 (+/- 2 about center)
        }        
        currentYPositions[i] -= currentYSpeeds[i];
      }

      //CHOOSE COLOUR OF CHARACTERS TO PRINT
      if (frameArrivedAtCenter[i] == true){                       //if char has passed the center, print winning condition colour
        colour = getColourToPrintBasedOnEndingFrame(endingFrame);
      }else{                                                      //else, use original colour
        colour = redColor;
      }
    }
    
    if (frameArrivedAtCenter[0] && frameArrivedAtCenter[1] && frameArrivedAtCenter[2]){        //if all frames has arrived at the center
      break;                                                                                   //Escape from while loop 
    }
  }

  //final jitter animation to bring frames to a stop
  oscillateWithDecreasingEnergyAnimation(currentXPositions, currentYPositions, currentCharacter, endingFrame);
  machineUpdates(endingFrame);
}

//LED MATRIX METHOD: TO GET INDEX OF MIN VALUE IN ARRAY METHOD
int minimum(float array[], int size){
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
  
  if (frameToPrint.equalsIgnoreCase(Jackpot)){                            //JACKPOT
    return blueColor;
    
  }else if (frameToPrint.equalsIgnoreCase(Win)){                      //EEE
    return greenColor;
    
  }else{                                                                //OTHERS
    return greenColor;
    
  }
}

//LED MATRIX METHOD: OSCILLATION ANIMATION ALGORITHMN
void oscillateWithDecreasingEnergyAnimation(int currentXPositions[], int currentYPositions[], int currentCharacter[], int endingFrame){

  //String frameToPrint = combo[endingFrame];
  uint16_t finalColour = getColourToPrintBasedOnEndingFrame(endingFrame);

  //oscillateAnimation
  int yPositionsOscillate[] = {8, 4, 8, 4, 8, 6};
  for (int j = 0; j < 6; j++){
    matrix.fillScreen(blackColor);              //Fill screen 'black'
    
    for (int i = 0; i < 3; i++){                              //for each character in cylinder 1, 2, 3 respectively
      //drawFrame
      char charToDraw = extractCharFromFrameList(currentCharacter[i], i);
      drawCharacter(currentXPositions[i], yPositionsOscillate[j], charToDraw, finalColour);
      
      //buzz SFX
      tone(speakerPin, 5000, 50);
    }
  }
}

//LED MATRIX METHOD: EXTRACT CHAR BY INDEX
char extractCharFromFrameList(int rowNumber, int colNumber){
  return combo[rowNumber].charAt(colNumber); //eg. combo[1] returns "NBS". combo[1].at(2) returns "B".
}


//LED MATRIX METHOD: DRAW CHARACTER, SET COLOR & POSITION
void drawCharacter(int xPos, int yPos, char characterToPrint, uint16_t color){
  matrix.setTextColor(color);
  matrix.setCursor(xPos, yPos);
  matrix.print(characterToPrint);
}

//SERVER MOTOR METHOD: TO DISPENSE COIN
void dispenseCoin(int amount){
  for(int i = 0; i < amount; i++){
    myservo.write(0);
    delay(180);
    myservo.write(intermediateSpinPos);
    delay(180);
    myservo.write(0);
  }
  totalCoinsInside -= amount;
}

//LCD, SERVER MOTOR METHOD: NECESSARY ACTIONS TAKEN BASED ON CONDITION
void machineUpdates(String endingFrame){
  if(endingFrame.equalsIgnoreCase(Jackpot)){
    LcdMessage(4);
    // TODO: JACKPOT ANIMATION
    dispenseCoin(totalCoinsInside);
    LcdMessage(5);
  }else if(endingFrame.equalsIgnoreCase(Win)){
    LcdMessage(3);
    // TODO: WIN ANIMATION
    dispenseCoin(currentBetAmt*winRate);
    if(creditAmt <= 0){                             //Message Type 1: Credit less than or equal to 0
      LcdMessage(0);
    }else{                                          //Message Type 2: When credit greater than 0
      LcdMessage(1);
    }
  }else{
    LcdMessage(2);
    // TODO: LOSE ANIMATION
    delay(2000);
    if(creditAmt <= 0){                             //Message Type 1: Credit less than or equal to 0
      LcdMessage(0);
    }else{                                          //Message Type 2: When credit greater than 0
      LcdMessage(1);
    }
  }
}
