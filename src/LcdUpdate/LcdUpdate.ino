//DFRobot.com
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//LCD Display: Declared Variable
int isObstaclePin = 7;
int isObstacle = HIGH;

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
  lcd.init();                      // initialize the lcd 
  pinMode(isObstaclePin, INPUT);
}

void loop()
{ 
  lcd.backlight();
  lcd.setCursor(1,1);
  isObstacle = digitalRead(isObstaclePin);
   
  if(isObstacle == LOW) {             // Obstacle detected
    Serial.println("On");
    coinInsert = true;
  } else {                            // No Obstacle
    Serial.println("Off");
    if(coinInsert){                   
      creditAmt++;                    // Incremeent Credit Amt
    }
    coinInsert = false;               //
  }

  switch(creditAmt){
    case 0 :  lcd.print("INSERT");
              lcd.setCursor(1,2);  
              lcd.print("COIN");
              break;
    default:  lcd.print("Credit : ");
              lcd.print(creditAmt);
              lcd.setCursor(1,2);   
              lcd.print("Bet : ");
              lcd.print(betAmt);
              break;
  }
}
