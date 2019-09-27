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
int spdWeights[] = {1,1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 1.9, 1.9, 1.9, 1.8, 1.7, 1.6, 1.5, 1.4, 1.3, 1.2, 1};
// Draw Character:  matrix.drawChar(x-pos, y-pos, character, color, background, size);
// Set Text Color:  matrix.Color333(r, g, b)
// Set Text Size:   matrix.setText(int)
// Set Text Wrap:   matrix.setTextWrap(boolean)
// Set Position:    matrix.setCursor(x-margin, y-margin)

  int numberOfRotations = 20;
  int startingFrame;
  int endingFrame;
  int ySpeed = 10;

  bool isSpinning = true;

void setup() {
  matrix.begin();
  matrix.setTextSize(3);        // size 1 == 8 pixels high
  matrix.setTextWrap(false);    // Don't wrap at end of line - will do ourselves
  Serial.begin(9600);
  randomSeed(analogRead(15));
}

void loop() {
    if (isSpinning == true) {
      startingFrame = random(300) % 10; //choose start frame
      endingFrame = random(300) % 10; //choose end frame
      Serial.println(startingFrame);
      playAnimation(startingFrame, endingFrame, numberOfRotations);

    }
    isSpinning = false;

}


void playAnimation(int startingFrame, int endingFrame, int numberOfRotations){
    //temp variables
    bool outsideFrame = true;

    int currentFrame;
    int currentXPos = 0;
    int currentYPos = 0;
    int startXPos = 1;
    int startYPos = -21; //starting pos = outside frame

    //initial conditions
    currentXPos = startXPos;
    currentYPos = startYPos;
    currentFrame = startingFrame;

    //start animation
    if (outsideFrame == true){
      currentYPos++; //shift by 1 row down, now img enters frame
      outsideFrame = false;
      char *str = combo[startingFrame].c_str(); //to remove?
    }

    //animating... rolling...
    for (int counter = 0; counter< numberOfRotations; ){
      //FILL SCREEN 'black'
      matrix.fillScreen(matrix.Color333(0,0,0));
      drawFrame(currentXPos, currentYPos, currentFrame);
      delay(0);
      currentYPos += ySpeed*spdWeights[counter]; //move frame down

      //check if img/frame exited matrix
      if (currentYPos >= matrix.height() + 21) { //text ht = 21
        currentYPos = startYPos; //wrap around/reset start Position
        outsideFrame = true;

        currentFrame = (currentFrame+1)%10; // swap to next frame
        counter++;
      }
    }

    //ending animation
    int endYPos = 6;
    for (int yPos = currentYPos; yPos <= endYPos; ){
      //print ROW of IMAGES
      matrix.fillScreen(matrix.Color333(0,0,0));
      yPos += ySpeed;
      drawFrame(currentXPos, yPos, endingFrame);
    }

    //jitter/shake . hardcoded
    matrix.fillScreen(matrix.Color333(0,0,0));
    drawFrame(currentXPos, 10, endingFrame);
    matrix.fillScreen(matrix.Color333(0,0,0));
    drawFrame(currentXPos, 4, endingFrame);
    matrix.fillScreen(matrix.Color333(0,0,0));
    drawFrame(currentXPos, 6, endingFrame);

}

void drawFrame(int currentXPos, int currentYPos, int frame){
  // DRAW Text
  uint8_t w = 0;
  uint8_t space = 24;
  // Characterise Selected String, extract String from String Array
  char *str = combo[frame].c_str();

  matrix.setCursor(currentXPos, currentYPos);
  for (w=0; w<3; w++) { // 3 = number of characters
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
    matrix.setCursor(space, currentYPos);
    space += space;
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
