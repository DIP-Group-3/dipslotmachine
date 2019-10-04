#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//LCD Display Variable
int isObstaclePin = 7;
int isObstacle = HIGH;
LiquidCrystal_I2C lcd(0x27,20,4);                // Set the LCD address to 0x27 for a 20 chars and 4 line display

//Bet Button Variables
const int betBtn = 12;
int betState = LOW;
bool betPressed = false;

//Spin Button Variables
const int spinBtn = 13;
int spinState = LOW;
bool spinPressed = false;

//Credit, Bet variable
int creditAmt =0, betAmt=0;

//IR sensor variable
bool coinInsert = false;

//Server Motor Variable
#define MotorPin 5
Servo servoMain;

void setup()
{
  Serial.begin(9600);
  servoMain.attach(MotorPin); 
  lcd.init();                                    // Initialize the LCD 
  pinMode(isObstaclePin, INPUT);
  pinMode(betBtn, INPUT);
  pinMode(spinBtn, INPUT);
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
    coinInsert = !coinInsert;  
  }else if(addAmt>0){                           //Scenario 2: Coin inserted when current credit !=0
    creditAmt += addAmt;
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
