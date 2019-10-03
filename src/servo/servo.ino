#include <Servo.h>
Servo servoMain;
int led = 13;
long int winCount = 0;

void setup() {         
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  servoMain.attach(5); // servo on digital pin 5
}

int minAngle = 20;
int maxAngle = 90;


void dispenseCoins(int numOfCoins) {
  for (int i = 0; i < numOfCoins; i++) {
    servoMain.write(maxAngle);
    delay(200);
    servoMain.write(minAngle);
    delay(200);
    servoMain.write(maxAngle);
    delay(200);
  }
}
void loop(){
  while(Serial.available() > 0){
    int winCount = Serial.parseInt();
    dispenseCoins(winCount);
  }W
}
