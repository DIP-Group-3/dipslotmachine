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
#define betBtn 12
#define spinBtn 13
#define isObstaclePin 14
#define reservePin 15

#define R1 24             // UPPER RGB DATA - TOP HALF OF DISPLAY
#define G1 25
#define B1 26
#define R2 27             // LOWER RGB DATA - BOTTOM HALF OF DISPLAY
#define G2 28
#define B2 29

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

//LED DESIGN GLOBAL VARIABLE;
uint16_t blackColor = matrix.Color333(0, 0, 0);
uint16_t redColor = matrix.Color333(255, 0, 0);
uint16_t blueColor = matrix.Color333(0, 255, 245);
uint16_t greenColor = matrix.Color333(76, 255, 56);

uint16_t x0=3, x1=4, x2=6, x3=7, x4=10, x5=15, x6= 19;

void setup(){
  matrix.begin();
}

void loop(){
  /*
    matrix.fillTriangle(x5, 9, x6, 13, x5, 17, blueColor);
    matrix.fillTriangle(x4, 18, x3, 21, x3, 18, blueColor); 
    matrix.fillTriangle(x4, 8, x3, 5, x3, 8, blueColor);
    matrix.fillRect(x3,9,8,9, greenColor);
    
    matrix.drawLine(x0,9, x2,9 ,redColor);
    matrix.drawLine(x1,11, x2,11 ,redColor);
    matrix.drawLine(x0,13, x2,13 ,redColor);
    matrix.drawLine(x1,15, x2,15 ,redColor);
    matrix.drawLine(x0,17, x2,17 ,redColor);
  */  
  for(int x0=3; x0 <63; x0+=3){
    matrix.fillScreen(blackColor);
    //drawTriangle(15, uint16_t 8, uint16_t 19, uint16_t 13, uint16_t 15, uint16_t 18, uint16_t redColor);
    matrix.fillTriangle(x5+x0, 9, x6+x0, 13, x5+x0, 17, blueColor);
    matrix.fillTriangle(x4+x0, 18, x3+x0, 21, x3+x0, 18, blueColor); 
    matrix.fillTriangle(x4+x0, 8, x3+x0, 5, x3+x0, 8, blueColor);
    matrix.fillRect(x3+x0,9,8, 9, greenColor);
    matrix.drawLine(x0,9, x2+x0,9 ,redColor);
    matrix.drawLine(x1+x0,11, x2+x0,11 ,redColor);
    matrix.drawLine(x0,13, x2+x0,13 ,redColor);
    matrix.drawLine(x1+x0,15, x2+x0,15 ,redColor);
    matrix.drawLine(x0,17, x2+x0,17 ,redColor);
    delayMicroseconds(1200);
  }
  
}
