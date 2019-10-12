#include <RGBmatrixPanel.h>

#define CLK 11    // USE THIS ON ARDUINO MEGA
#define OE   9    // OUTPUT ENABLE
#define LAT 10    // LATCH SIGNAL MARKS THE END OF A ROW OF DATA
#define A   A0    // ROW SELECT: WHICH TWO ROWS CURRENTLY LIT
#define B   A1
#define C   A2
#define D   A3
#define R1  24    // UPPER RGB DATA - TOP HALF OF DISPLAY
#define G1  25
#define B1  26
#define R1  27    // LOWER RGB DATA - BOTTOM HALF OF DISPLAY
#define G2  28
#define B2  29
#define reservePin 15

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

int waterfallColumns[32]; 
int waterfallStartingPos[32];
int waterfallRotations = 40;
uint16_t waterfallColor1 = matrix.Color333(0, 210, 84);
uint16_t waterfallColor2 = matrix.Color333(0, 202, 202);
uint16_t waterfallColor3 = matrix.Color333(161, 0, 202);

int radiationRotations = 20;
uint16_t radiatoinColors[4] = {matrix.Color333(213, 0, 64), matrix.Color333(206, 0, 185),
                               matrix.Color333(161, 0, 202), matrix.Color333(99, 0, 198)};

void setup() {
  // put your setup code here, to run once:
  matrix.begin();
  randomSeed(analogRead(reservePin));
  waterfall();
  radiation();  // Tedious but gaurantee will work
  radiation1(); // Simple but not sure if it will cause any problem
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void waterfall(){
  // Generate waterfall columns             This gives the X0, X1
  for(int i = 0; i < 32; i++){
    waterfallColumns[i] = (2*i+1);
  }
  // Generate waterfall starting positions  This gives the Y0
  for(int i = 0; i < 32; i++){
    waterfallStartingPos[i] = random(0,31);
  }
  // Rolling animation
  for(int i = 0; i < waterfallRotations; i++){
    for(int j = 0; j < 32; j++){
      // Get the starting and ending pos
      int yStart = waterfallStartingPos[j] + i;
      int yEnd = yStart - 32; // 32 is the length of each waterfall
      if(yEnd < 0 and yStart < 31){
        yEnd = 0;
      }else if(yStart > 31){
        yStart = 31;
      }else if(yEnd > 31){
        yStart = yStart - 64;
        yEnd = 0;
      }
      // Changing color
      uint16_t currColor;
      int section1 = 6 + sin(j);
      int section2 = 12 + sin(j);
      if(yStart < section1){
        matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], yEnd, waterfallColor1);
      }else if (yStart < section2){
        matrix.drawLine(waterfallColumns[j], section1-1, waterfallColumns[j], yEnd, waterfallColor1);
        matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], section1, waterfallColor2);
      }else{
        if(yEnd < section1){
          matrix.drawLine(waterfallColumns[j], section1-1, waterfallColumns[j], yEnd, waterfallColor1);
          matrix.drawLine(waterfallColumns[j], section2-1, waterfallColumns[j], section1, waterfallColor2);
          matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], section2, waterfallColor3);
        }else if(yEnd < section2){
          matrix.drawLine(waterfallColumns[j], section2-1, waterfallColumns[j], yEnd, waterfallColor2);
          matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], section2, waterfallColor3);
        }else{
          matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], yEnd, waterfallColor3);
        }
      }
      // Serial.print();
    }
    Serial.print('\n');
    delay(0);
  }
}

void radiation(){
  for(int i = 0; i < radiationRotations; i ++){
    for(int x = 0; x < 63; x++){
      for(int y = 0; y < 31; y++){
        int d = (int) sqrt((x - 15)^2 + (y-31)^2);
        if(d <= 5){
          drawPixel(x, y, radiatoinColors[(0+i)%4]);
        }else if (d <= 10){
          drawPixel(x, y, radiatoinColors[(1+i)%4]);
        }else if(d <= 15){
          drawPixel(x, y, radiatoinColors[(2+i)%4]);
        }else{
          drawPixel(x, y, radiatoinColors[(3+i)%4]);
        }
      }
    }
    delay(0);
  }
}

void radiation1(){ 
  for(int i = 0; i < radiationRotations; i++){
    // section 1 circle
    matrix.fillCircle(15,31,5,radiatoinColors[(0+i)%4]);
    
    // section 2 circle
    for(int r = 6; r < 11; r++){
      matrix.drawCircle(15,31,r,radiationColors[(1+i)%4]);
    }

    // section 3 circle
    for(int r = 11; r < 16; r++){
      matrix.drawCircle(15,31,r,radiationColors[(2+i)%4]);
    }

    // section 4 circle
    for(int r = 16; r < 32; r++){
      matrix.drawCircle(15,31,r,radiationColors[(3+i)%4]);
    }
  }
}
