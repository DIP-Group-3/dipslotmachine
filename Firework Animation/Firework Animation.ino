#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
 
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

uint16_t blackColor = matrix.Color333(0, 0, 0);
uint16_t redColor = matrix.Color333(255, 0, 0);
uint16_t blueColor = matrix.Color333(0, 255, 245);
uint16_t greenColor = matrix.Color333(76, 255, 56);

const int set1XStart = 4, set1XEnd = 17;
const int set2XStart = 25, set2XEnd = 38;
const int set3XStart = 46, set3XEnd = 59;
const int yMax = 20, yMin =2; 
  bool roll = true;
 
void setup() {
  matrix.begin();
}
 
void loop() {

  if(roll){
  firework( random(set1XStart, set1XEnd), random(yMin, yMax), matrix.Color333(7, 0, 0), matrix.Color333(3, 0, 0), 10);
  firework( random(set3XStart, set3XEnd), random(yMin, yMax), matrix.Color333(0, 7, 0), matrix.Color333(0, 3, 0), 20);
  firework( random(set2XStart, set2XEnd), random(yMin, yMax), matrix.Color333(0, 0, 7), matrix.Color333(0, 0, 3), 15);
  
  firework( random(set1XStart, set1XEnd), random(yMin, yMax), matrix.Color333(7, 3, 1), matrix.Color333(3, 1, 0), 20);
  firework( random(set3XStart, set3XEnd), random(yMin, yMax), matrix.Color333(7, 7, 7), matrix.Color333(3, 1, 0), 16);
  firework( random(set2XStart, set2XEnd), random(yMin, yMax), matrix.Color333(3, 0, 7), matrix.Color333(1, 0, 3), 18);
 
  firework( random(set1XStart, set1XEnd), random(yMin, yMax), matrix.Color333(5, 3, 0), matrix.Color333(7, 1, 1), 20);
  firework( random(set3XStart, set3XEnd), random(yMin, yMax), matrix.Color333(7, 0, 0), matrix.Color333(7, 3, 0), 16);
  firework( random(set2XStart, set2XEnd), random(yMin, yMax), matrix.Color333(3, 7, 7), matrix.Color333(7, 3, 3), 18);
  roll =false;
  }

}
 
void firework(byte x, byte y, uint16_t lineColor, uint16_t radColor, uint8_t delayTime) {
   
  for( byte i=32; i>y; i--) {
    matrix.drawLine(x, i, x, (i+1), lineColor);
    delay(delayTime);
    matrix.drawLine(x, i, x, (i+1), blackColor);
  }
  delay(delayTime);
  matrix.drawCircle(x, y, 1, lineColor); delay(delayTime*3);
  matrix.drawCircle(x, y, 1, blackColor);
 
  for ( byte j=1;j<4; j++) {
    matrix.drawLine(x, (y-5)-j, x, (y-4)-j, lineColor);
    matrix.drawLine(x, (y+2)+j, x, (y+3)+j, lineColor);
    matrix.drawLine((x-5)-j, y, (x-4)-j, y, lineColor);
    matrix.drawLine((x+2)+j, y, (x+3)+j, y, lineColor);
 
    matrix.drawLine((x+1)+j, (y+1)+j, (x+3)+j, (y+3)+j, radColor);
    matrix.drawLine((x-1)-j, (y+1)+j, (x-3)-j, (y+3)+j, radColor);
    matrix.drawLine((x+1)+j, (y-1)-j, (x+3)+j, (y-3)-j, radColor);
    matrix.drawLine((x-1)-j, (y-1)-j, (x-3)-j, (y-3)-j, radColor);
     
    delay(delayTime*2);
     
    matrix.drawLine(x, (y-5)-(j-1), x, (y-4)-(j-1), blackColor);
    matrix.drawLine(x, (y+2)+(j-1), x, (y+3)+(j-1), blackColor);
    matrix.drawLine((x-5)-(j-1), y, (x-4)-(j-1), y, blackColor);
    matrix.drawLine((x+2)+(j-1), y, (x+3)+(j-1), y, blackColor);
 
    matrix.drawLine((x+1)+(j-1), (y+1)+(j-1), (x+3)+(j-1), (y+3)+(j-1), blackColor);
    matrix.drawLine((x-1)-(j-1), (y+1)+(j-1), (x-3)-(j-1), (y+3)+(j-1), blackColor);
    matrix.drawLine((x+1)+(j-1), (y-1)-(j-1), (x+3)+(j-1), (y-3)-(j-1), blackColor);
    matrix.drawLine((x-1)-(j-1), (y-1)-(j-1), (x-3)-(j-1), (y-3)-(j-1), blackColor);
    delay(delayTime*2);
 
    matrix.drawLine(x, (y-5)-j, x, (y-4)-j, blackColor);
    matrix.drawLine(x, (y+2)+j, x, (y+3)+j, blackColor);
    matrix.drawLine((x-5)-j, y, (x-4)-j, y, blackColor);
    matrix.drawLine((x+2)+j, y, (x+3)+j, y, blackColor);

    matrix.drawLine((x+1)+j, (y+1)+j, (x+3)+j, (y+3)+j, blackColor);
    matrix.drawLine((x-1)-j, (y+1)+j, (x-3)-j, (y+3)+j, blackColor);
    matrix.drawLine((x+1)+j, (y-1)-j, (x+3)+j, (y-3)-j, blackColor);
    matrix.drawLine((x-1)-j, (y-1)-j, (x-3)-j, (y-3)-j, blackColor);
  }
}
