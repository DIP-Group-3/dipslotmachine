#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <RGBmatrixPanel.h>

//PIN DEFINITIONS
#define A A0   // ROW SELECT: WHICH TWO ROWS CURRENTLY LIT
#define B A1
#define C A2
#define D A3

#define MotorPin 5
#define speakerPin 7
#define OE 9   // OUTPUT ENABLE
#define LAT 10 // LATCH SIGNAL MARKS THE END OF A ROW OF DATA
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define betBtn 12
#define spinBtn 13
#define isObstaclePin 14
#define reservePin 15

#define R1 24 // UPPER RGB DATA - TOP HALF OF DISPLAY
#define G1 25
#define B1 26
#define R1 27 // LOWER RGB DATA - BOTTOM HALF OF DISPLAY
#define G2 28
#define B2 29

//ARRAY OF COMBOS & SELECT, Combinations of all possible Frames (10)
String combo[] = {"EEE", "NBS", "IEM", "ADM", "SCE", "NBS", "SCE", "ADM", "EEE", "SCE"};

//Array to determine scroll speed
//ySpeed, or scroll speed is proportional to weights (20)
float spdWeights[] = {0.6, 0.7, 0.8, 0.8, 0.9, 1.0, 1.1, 1.2, 1.5, 1.5, 1.5, 1.5, 1.5, 1.4, 1.3, 1.2, 1.1, 1.1, 1.0, 1.0, 1};

//construct 64x32 LED MATRIX panel
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

//Global Variables
int numOfFrames = 10;
int numberOfRotations = 20;
float ySpeed = 10; //vertical scrollSpeed
bool isSpinning = true; //spin button to trigger status;
int startingFrame; 
int endingFrame;

//LCD Display Variable
int isObstacle = HIGH;
LiquidCrystal_I2C lcd(0x27,20,4);                // Set the LCD address to 0x27 for a 20 chars and 4 line display

//Bet Button Variables
int betState = LOW;
bool betPressed = false;

//Spin Button Variables
int spinState = LOW;
bool spinPressed = false;

//Credit, Bet variable
int creditAmt =0, betAmt=0;

//IR sensor variable
bool coinInsert = false;

//Server Motor Variable
Servo myservo;
int totalCoinsInside = 3;
int intermediateSpinPos = 90;

void setup()
{
  Serial.begin(9600);
  servoMain.attach(MotorPin); 
  lcd.init();                                    // Initialize the LCD 
  pinMode(isObstaclePin, INPUT);
  pinMode(betBtn, INPUT);
  pinMode(spinBtn, INPUT);
  
  //Set-up code for matrix
  matrix.begin();
  //set text properties
  matrix.setTextSize(3); // size 1 -> hxw = 7x4. size = 3 -> hxw = (7*3)x(4*3) = 21x12
  matrix.setTextWrap(false);  // Don't wrap at end of line - will do ourselves

  // *MUST SEED using Analog input from UNUSED_PIN. analogRead(**UNUSED_PIN**). Pin15 is used here temporarily
  randomSeed(analogRead(reservePin));

  //LCD Display "Insert Coin" Message
  LcdMessage(0);
}

void loop()
{ 
  lcd.backlight();
  isObstacle = digitalRead(isObstaclePin);
  if(isObstacle == LOW) {                       // Obstacle detected 
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
}

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

void activateSpin(){                                            //Method: Activate LED Matrix
  spinPressed = false;                                              
  creditAmt -= betAmt;                                              //Deducted credit based on bet amount
  if(creditAmt <=0){
    creditAmt = 0;                                                  //Credit amount set back to 0
    betAmt = 0;                                                     //Bet amount set back to 0
    LcdMessage(0);                                                  //Display "Insert Coin" Message when credit less than 0    
  }else{
    betAmt = 1;                                                     //Bet amount set to 1
    LcdMessage(1);                                                  //Display remaining credit and bet amounts in LCD
  }
  activateLED();
}

bool spinAvail(){
  spinState = digitalRead(spinBtn);                               //Read  Spin Btn Input
  if(spinState == HIGH){
    spinPressed = true;
  }else if (spinState == LOW && creditAmt > 0 && spinPressed){
    Serial.println("Spin");
    return true;
  }else{
    spinPressed = false;
    return false;
  }
}

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

void activateLED(){
  startingFrame = random(300) % numOfFrames; //choose random start frame
  endingFrame = random(300) % numOfFrames;   //choose random end frame

  playAnimation(startingFrame, endingFrame, numberOfRotations);
}

void playAnimation(int startingFrame, int endingFrame, int numberOfRotations)
{
  int currentFrame;
  int currentXPos = 0, currentYPos = 0;
  int startXPos = 1; //todo: missing animation: OXO -> OOX -> X00
  int yPosCenter = 6, yPosTop = -21; //starting pos = outside frame

  //initial conditions
  currentXPos = startXPos;
  currentYPos = yPosCenter;//startYPos;
  currentFrame = startingFrame;
  
  //begin animation...is rolling...for numberOfRotations times
  for (int counter = 0; counter < numberOfRotations;){
    matrix.fillScreen(matrix.Color333(0, 0, 0));                                      //FILL SCREEN 'black'
    drawFrame(currentXPos, currentYPos, currentFrame, matrix.Color333(255,0,0));
    delay(0);
    currentYPos += round(ySpeed * spdWeights[counter]);                               //Move frame down by scrollSpeed scaled by weights
    
    //Check if frame exited matrix
    if (currentYPos >= matrix.height() + 21){                                         //Text height = 21                       
      currentYPos = yPosTop;                                                          //Wrap around/reset to start Position

      currentFrame = (currentFrame + 1) % numOfFrames;                                //Swap to next frame
      counter++;
      tone(piezoPin, 1000, 100);                                                      //Beep sfx as frame enters matrix
    }
  }

  //Ending animation 
  int endYPos = 6;
  for (; currentYPos <= endYPos;){
    //print ROW of IMAGES
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    currentYPos += ySpeed;
    drawFrame(currentXPos, currentYPos, endingFrame, matrix.Color333(255,0,0));
  }

  jitterAnimation(endingFrame, currentXPos, currentYPos);

  // dispense coins 
  if(endingFrame == 0){
    dispenseCoin(2 * betAmt);   // calculate the amount 
    totalCoinsInside -= (2 * betAmt);
  }else if (endingFrame == 2){
    dispenseCoin(totalCoinsInside); // calculate the amount, dispense all
    totalCoinsInside = 0;
  }
}

void jitterAnimation(int endingFrame, int currentXPos, int currentYPos){
  char *frameStr = combo[endingFrame].c_str();
  char * iem = "IEM";
  char * eee = "EEE";
  uint16_t colour;
  
  if (!strcmp(iem, frameStr)) {                       //JACKPOT Condition : IEM
    Serial.print("JACKPOT, IEM\n");
    colour = matrix.Color333(0, 255, 245);
  }else if (!strcmp(eee, frameStr)) {                 //WINING Condition : EEE
    Serial.print("EEE\n");
    colour = matrix.Color333(76, 255, 56);
  } else {                                            //OTHER Conditions
    colour = matrix.Color333(0, 254, 0);
  }

  //jitter/shake stop animation
  int yPositionsJitter[] = {10, 4, 10, 4, 6};

  for (int i = 0; i < 5; i++){
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    currentYPos = yPositionsJitter[i];
    drawFrame(currentXPos, currentYPos, endingFrame, colour);
    Serial.print(colour);
    tone(piezoPin, 5000, 50);
  }
}


void drawFrame(int currentXPos, int currentYPos, int frame, uint16_t colour){
  // DRAW Text
  uint8_t w = 0;
  uint8_t space = 24;
  
  // Characterise Selected String, extract String from String Array
  char *str = combo[frame].c_str();

  matrix.setCursor(currentXPos, currentYPos);
  for (w = 0; w < 3; w++) {                         // 3 = number of characters
    matrix.setTextColor(colour);
    matrix.print(str[w]);
    matrix.setCursor(space, currentYPos);
    space += space;
  }
}

void dispenseCoin(int amount){
  for(int i = 0; i < amount; i++){
    myservo.write(0);
    delay(180);
    myservo.write(intermediateSpinPos);
    delay(180);
    myservo.write(0);
  }
}