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

uint16_t mainRck_EL_R1R = 12, mainRck_ES_R2R = 14, mainRck_EL_R3R = 16, mainRck_ES_R4R = 18, mainRck_EL_R5R = 20,
         mainRck_EL_Start = -15, mainRck_ES_Start = -13, mainRck_EL_Length = 4, mainRck_ES_Length = 2,
         mainRck_Rect_X = -11, mainRck_Rect_Y = 12, mainRck_Rect_W = 5, mainRck_Rect_H = 9,
         mainRck_CenTri_X1 = -11, mainRck_CenTri_X2 = -9,
         mainRck_TT_Y1 = 9, mainRck_TT_Y2 = 11, mainRck_BT_Y1 = 21, mainRck_BT_Y2 = 23,
         mainRck_RT_X1 = -6, mainRck_RT_Y1 = 12, mainRck_RT_X2 = -2, mainRck_RT_Y2 = 16,  mainRck_RT_Y3 = 20;

uint16_t leftRect_W = 4, leftRect_H = 5, leftRect_X = -61,
         leftRck_EL_Start = -63, leftRck_ES_Start = -62,
         leftRck_EL_Length = 2, leftRck_ES_Length = 1,
         leftRck_CenTri_X1 = -61, leftRck_CenTri_X2 = -60,
         leftRck_RT_X1=-57, leftRck_TT_X1 = -61, leftRck_TT_X2 = -60;

uint16_t leftRck_EL_R1R = 3, leftRck_EL_R2R = 5, leftRck_EL_R3R = 7,
         leftRck_EL_R4R = 24, leftRck_EL_R5R = 26, leftRck_EL_R6R = 28,
         leftRect_top_Y = 3, leftRect_bottom_Y =  24,
         leftRck_TT1_Y1 = 1, leftRck_TT1_Y2 = 2, leftRck_TT2_Y1 = 22, leftRck_TT2_Y2 = 23,
         leftRck_BT1_Y1 = 8, leftRck_BT1_Y2 = 9, leftRck_BT2_Y1 = 29, leftRck_BT2_Y2 = 30,
         leftRck_RT_Y1 = 3, leftRck_RT_X2 = -55, leftRck_RT_Y2 = 5, leftRck_RT_Y3 = 7,
         leftRck_RT_Y4 = 24, leftRck_RT_Y5 = 26, leftRck_RT_Y6 = 28;

uint16_t firstChar_X=-53, secondChar_X=-40, thirdChar_X=-27, Char_Y =9;

void setup(){
  matrix.begin();
}

void loop(){
/*
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
*/
  for(int index = -2; index < 129; index +=4 ){
    //Clear LED Screen
    matrix.fillScreen(blackColor);

    //Draw Big Rocket
    matrix.fillTriangle(mainRck_RT_X1+index, mainRck_RT_Y1, mainRck_RT_X2+index, mainRck_RT_Y2, mainRck_RT_X1+index, mainRck_RT_Y3, blueColor);
    matrix.fillTriangle(mainRck_CenTri_X1+index, mainRck_TT_Y1, mainRck_CenTri_X2+index, mainRck_TT_Y2, mainRck_CenTri_X1+index, mainRck_TT_Y2, blueColor);
    matrix.fillTriangle(mainRck_CenTri_X1+index, mainRck_BT_Y1, mainRck_CenTri_X2+index, mainRck_RT_Y2, mainRck_CenTri_X1+index, mainRck_BT_Y1, blueColor);

    matrix.fillRect(mainRck_Rect_X+index, mainRck_Rect_Y, mainRck_Rect_W, mainRck_Rect_H, greenColor);

    matrix.drawLine(mainRck_EL_Start+index, mainRck_EL_R1R, mainRck_EL_Start+mainRck_EL_Length+index,mainRck_EL_R1R, redColor);
    matrix.drawLine(mainRck_ES_Start+index, mainRck_ES_R2R, mainRck_ES_Start+mainRck_EL_Length+index,mainRck_ES_R2R, redColor);
    matrix.drawLine(mainRck_EL_Start+index, mainRck_EL_R3R, mainRck_EL_Start+mainRck_EL_Length+index,mainRck_EL_R3R, redColor);
    matrix.drawLine(mainRck_ES_Start+index, mainRck_ES_R4R, mainRck_ES_Start+mainRck_EL_Length+index,mainRck_ES_R4R, redColor);
    matrix.drawLine(mainRck_EL_Start+index, mainRck_EL_R5R, mainRck_EL_Start+mainRck_EL_Length+index,mainRck_EL_R5R, redColor);

    //Draw Char
    matrix.drawChar(firstChar_X, Char_Y, 'I', blueColor, blackColor, 2);
    matrix.drawChar(secondChar_X, Char_Y, 'E', blueColor, blackColor, 2);
    matrix.drawChar(thirdChar_X, Char_Y, 'M', blueColor, blackColor, 2);
    

    //Draw Small Rockets
    matrix.fillTriangle(leftRck_RT_X1+index, leftRck_RT_Y1, leftRck_RT_X2+index, leftRck_RT_Y2, leftRck_RT_X1+index, leftRck_RT_Y3, blueColor);
    matrix.fillTriangle(leftRck_RT_X1+index, leftRck_RT_Y4, leftRck_RT_X2+index, leftRck_RT_Y5, leftRck_RT_X1+index, leftRck_RT_Y6, blueColor);

    matrix.fillRect(leftRect_X+index, leftRect_top_Y, leftRect_W, leftRect_W, greenColor);
    matrix.fillRect(leftRect_X+index, leftRect_bottom_Y, leftRect_W, leftRect_W, greenColor);

    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_TT1_Y1, leftRck_CenTri_X2+index, leftRck_TT1_Y2, leftRck_CenTri_X1+index, leftRck_TT1_Y2, blueColor);
    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_TT2_Y1, leftRck_CenTri_X2+index, leftRck_TT2_Y2, leftRck_CenTri_X1+index, leftRck_TT2_Y2, blueColor);

    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_BT1_Y1, leftRck_CenTri_X2+index, leftRck_BT1_Y2, leftRck_CenTri_X1+index, leftRck_BT1_Y1, blueColor);
    matrix.fillTriangle(leftRck_CenTri_X1+index, leftRck_BT2_Y1, leftRck_CenTri_X2+index, leftRck_BT2_Y2, leftRck_CenTri_X1+index, leftRck_BT2_Y1, blueColor);

    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R1R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R1R ,redColor);
    matrix.drawLine(leftRck_ES_Start+index, leftRck_EL_R2R, leftRck_ES_Start+leftRck_EL_Length+index, leftRck_EL_R2R ,redColor);
    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R3R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R3R ,redColor);
    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R4R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R4R ,redColor);
    matrix.drawLine(leftRck_ES_Start+index, leftRck_EL_R5R, leftRck_ES_Start+leftRck_EL_Length+index, leftRck_EL_R5R ,redColor);
    matrix.drawLine(leftRck_EL_Start+index, leftRck_EL_R6R, leftRck_EL_Start+leftRck_EL_Length+index, leftRck_EL_R6R ,redColor);

    if(index == 62){
      delay(100000);
    }else{
      delayMicroseconds(2000);
    }
  }


}
