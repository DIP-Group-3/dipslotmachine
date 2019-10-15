#include <Servo.h>

//global variables
Servo servoMain;
int servoEndAngle = 20;
int servoStartAngle = 90;

void setup() {         
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  servoMain.attach(5); // servo on digital pin 5
  servoMain.write(startAngle); //initialise servo position
}

void dispenseCoins(int numOfCoins) {
  for (int i = 0; i < numOfCoins; i++) {
    servoMain.write(servoStartAngle);
    delay(200);
    servoMain.write(servoEndAngle);
    delay(200);
    //servoMain.write(servoStartAngle); //not needed
    //(200);
  }
}
void loop(){
  while(Serial.available() > 0){
    int winCount = Serial.parseInt();
    dispenseCoins(winCount);
  }
}
