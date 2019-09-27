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

//ARRAY OF COMBOS & SELECT
String combo[] = {"EEE", "NBS", "IEM", "ADM", "SCE", "NBS", "SCE", "ADM", "EEE","SCE"};

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

// Draw Character:  matrix.drawChar(x-pos, y-pos, character, color, background, size);
// Set Text Color:  matrix.Color333(r, g, b)
// Set Text Size:   matrix.setText(int)
// Set Text Wrap:   matrix.setTextWrap(boolean)
// Set Position:    matrix.setCursor(x-margin, y-margin)

/*
  int currentXPos = 0;
  int currentYPos = 0;
  int startXPos = 1;
  int startYPos = -21; //5; //starting pos = outside frame
  int ySpeed = 10;
  int startingFrame;
  int endingFrame;
  int RandIndex = 0; //to delete
  int numberOfRotations = 10;
  bool outsideFrame = true;
*/

  int numberOfRotations = 10;
  int startingFrame;
  int endingFrame;
  int ySpeed = 10;
  //to modify
  bool isSpinning = true;

void setup() {
  matrix.begin();
  matrix.setTextSize(3);        // size 1 == 8 pixels high
  matrix.setTextWrap(false);    // Don't wrap at end of line - will do ourselves
 /*
  currentXPos = startXPos;
  currentYPos = startYPos; */
}

void loop() {
    if (isSpinning == true) {
      startingFrame = rand() % 10;//selectFrame(combo); //potential bug**
      endingFrame = rand() % 10;//selectFrame(combo);

      playAnimation(startingFrame, endingFrame, numberOfRotations);

    }
    isSpinning = false;
    //matrix.print("END");
}
void playAnimation(int startingFrame, int endingFrame, int numberOfRotations){
    //temp variables
    bool outsideFrame = true;
    int currentXPos = 0;
    int currentYPos = 0;
    int startXPos = 1;
    int startYPos = -21; //starting pos = outside frame

    //initial conditions
    currentXPos = startXPos;
    currentYPos = startYPos;

    //start animation
    if (outsideFrame == true){
      currentYPos++; //shift by 1 row down, now img enters frame
      outsideFrame = false;
      char *str = combo[startingFrame].c_str();
    }
    
    //animating...
    for (int counter = 0; counter< numberOfRotations; ){
      // Characterise Selected String
      char *str = combo[startingFrame].c_str();

      //FILL SCREEN 'black'
      matrix.fillScreen(matrix.Color333(0,0,0));

      // DRAW Text
      uint8_t w = 0;
      uint8_t space = 24;
      matrix.setCursor(currentXPos, currentYPos);
      for (w=0; w<3; w++) { // 3 = number of characters
        matrix.setTextColor(Wheel(w));
        matrix.print(str[w]);
        matrix.setCursor(space, currentYPos);
        space += space;
      }
      delay(0);
      currentYPos += ySpeed;

      //check if img exited endingFrame
      if (currentYPos >= matrix.height() + 21) { //text ht = 21
        currentYPos = startYPos; //wrap around/reset start Position
        outsideFrame = true;
        counter++;
      }
    }
}


// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}
