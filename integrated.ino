//DFRobot.com

//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x3F for a 20 chars and 4 line display

Servo servoMain;
int led = 13;
long int winCount = 0;

int LED = 13; // Use the onboard Uno LED
int isObstaclePin = 7; // This is our input pin
int isObstacle = HIGH; // HIGH MEANS NO OBSTACLE

void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  servoMain.attach(5); // servo on digital pin 5
  lcd.init();                      // initialize the lcd 
  pinMode(LED, OUTPUT);
  pinMode(isObstaclePin, INPUT);
  Serial.begin(9600);
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0); // set cursor to column 4, row 0
  lcd.print("16 x 2 Display");
  lcd.setCursor(1,1); // set cursor to column 3, row 1
  lcd.print("I2C addr: 0x3F");
}

void loop()
{
  isObstacle = digitalRead(isObstaclePin);
  if (isObstacle == LOW)
  {
  Serial.println("OBSTACLE!!, OBSTACLE!!");
  digitalWrite(LED, HIGH);
  }
  else
  {
  Serial.println("clear");
  digitalWrite(LED, LOW);
  }
  delay(000);
    {
    servoMain.write(120);
     delay(10);
     while (Serial.available() > 0) {
      int winCount = Serial.parseInt(); //manual serial input for how many times it needs to dispense
      while (winCount>0) {
        servoMain.write(40);  // Turn Servo Left to 45 degrees
        delay(170);          // Wait 1 second
        servoMain.write(120);   // Turn Servo Left to 0 degrees
        delay(170);          // Wait 1 second
        winCount--;  // wait for a second
      }
    }
  }
  
}
