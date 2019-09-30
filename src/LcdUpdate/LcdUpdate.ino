//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//LCD Display: Declared Variable
int isObstaclePin = 7;
int isObstacle = HIGH;

//Buttons Declared Variable
int betBtn = 12;
int betPress = HIGH;
int spinBtn = 13;
int spinPress = HIGH;

//Server Motor Variable
#define MotorPin 5

Servo servoMain;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display

int creditAmt =0, betAmt=0;
long int winCount = 0;
bool coinInsert = false;

void setup()
{
  Serial.begin(9600);
  servoMain.attach(MotorPin); 
  lcd.init();                      // Initialize the lcd 
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
    Serial.println("On");
    coinInsert = true;
  } else if(isObstacle == HIGH && coinInsert){  // No Obstacle AND coinInsert == True
    Serial.println("Off");
    updateCredit(1);                            // Incremeent Credit Amt
    LcdMessage(1);
  }
  
  betPress = digitalRead(betBtn);
  spinPress = digitalRead(spinBtn);
  if(betAvail()){                               //Check if bet increment could be done
    updateBet()
  }else if(spinAvail()){                        //Ceck if spin could be done
    //Spin Frames
  }
}

void updateCredit(int addAmt){       //Method: Update Credit Amount
  if(addAmt>0 && creditAmt == 0){    //Scenario 1: Coin inserted when current credit == 0
    betAmt = 1;
    creditAmt += addAmt;
    coinInsert = !coinInsert;  
  }else if(addAmt>0){                //Scenario 2: Coin inserted when current credit !=0
    creditAmt += amt;
    coinInsert = !coinInsert;  
  }else{                             //Scenario 3: Credit deducted when played
    creditAmt += amt;               
    if(creditAmt <= 0){                   //Message Type 1: Credit less than or equal to 0
      LcdMessage(0);
    }else{                                //Message Type 2: When credit greater than 0
      LcdMessage(1);
    }
  }
}
void updateBet(){                   //Method: Update Bet Amount when Bet Button Pressed
  if(betAmt>=3 || betAmt >= creditAmt){                    //Scenario 1: Current bet amount equal to/greater than 3
    betAmt = 1;
  }else{                            //Scenario 2: Current bet amount less than 3
     betAmt++;
  }
}

bool betAvail(){
  if(betBtn == LOW && creditAmt > 0){   //Bet Btn Pressed && credit Amt greater than 0
    return true;
  }else{
    return false;
  }
}

bool spinAvail(){
  if(spinBtn == LOW && betAmt > 0){     //Spin Btn Pressed && Bet Amt greater than 0
    return true;
  }else{
    return false;
  }
}

void LcdMessage(int scenario){
  lcd.clear();                          //Clear current displayed frame
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
