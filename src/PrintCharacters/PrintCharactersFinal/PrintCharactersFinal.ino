#include <RGBmatrixPanel.h>


//PIN DEFINITIONS
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

//ARRAY OF COMBOS & SELECT, Combinations of all possible Frames (10)
String combo[] = {"EEE", "NBS", "IEM", "ADM", "SCE", "NBS", "SCE", "ADM", "EEE","SCE"};

//Array to determine scroll speed
//ySpeed, or scroll speed is proportional to weights (20)
int spdWeights[] = {1,1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 1.9, 1.9, 1.9, 1.8, 1.7, 1.6, 1.5, 1.4, 1.3, 1.2, 1};

//construct 64x32 LED MATRIX panel
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

//Global Variables
int numOfFrames = 10;
int numberOfRotations = 20;
int ySpeed = 10; //vertical scrollSpeed





void setup() {

  //Set-up code for matrix
  matrix.begin();
  //set text properties
  matrix.setTextSize(3);        // size 1 -> hxw = 7x4. size = 3 -> hxw = (7*3)x(4*3) = 21x12
  matrix.setTextWrap(false);    // Don't wrap at end of line - will do ourselves

  // *MUST SEED using Analog input from UNUSED_PIN. analogRead(**UNUSED_PIN**). Pin15 is used here temporarily
  randomSeed(analogRead(15));
}

void loop() {

  //Example code on how to implement rolling animation
  //{...before}: spin button is pressed
  bool isSpinning = true; //spin button sets isSpinning = true

  if (isSpinning == true) {
    int startingFrame = random(300) % numOfFrames; //choose random start frame
    int endingFrame = random(300) % numOfFrames; //choose random end frame

    playAnimation(startingFrame, endingFrame, numberOfRotations);
  }
  isSpinning = false;
}


void playAnimation(int startingFrame, int endingFrame, int numberOfRotations){
    bool outsideMatrix = true;
    int currentFrame;
    int currentXPos = 0;
    int currentYPos = 0;
    int startXPos = 1;
    int startYPos = -21; //starting pos = outside frame

    //initial conditions
    currentXPos = startXPos;
    currentYPos = startYPos;
    currentFrame = startingFrame;


    //9:49PM 27/9/2019 FRIDAY, Nadine: outsideMatrix variable isn't necessary for wrapping logic
    //check if it's really the case on LED before removing

    //start animation
    if (outsideMatrix == true){
      currentYPos++; //shift by 1 row down, now img enters matrix
      outsideMatrix = false; //frame is in matrix

      // Characterise Selected String, extract String from String Array
      char *str = combo[startingFrame].c_str(); //maybe not needed. to remove? remove aft testing if really not req
    }

    //animating...is rolling...for numberOfRotations times
    for (int counter = 0; counter < numberOfRotations; ){

      matrix.fillScreen(matrix.Color333(0,0,0)); //FILL SCREEN 'black'
      drawFrame(currentXPos, currentYPos, currentFrame);
      delay(0);
      currentYPos += ySpeed*spdWeights[counter]; //move frame down by scrollSpeed scaled by weights

      //check if frame exited matrix
      if (currentYPos >= matrix.height() + 21) { //text ht = 21
        currentYPos = startYPos; //wrap around/reset to start Position
        outsideMatrix = true;

        currentFrame = (currentFrame+1)%numOfFrames; // swap to next frame
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

    //jitter/shake stop animation, hardcoded
    matrix.fillScreen(matrix.Color333(0,0,0));
    drawFrame(currentXPos, 10, endingFrame);
    matrix.fillScreen(matrix.Color333(0,0,0));
    drawFrame(currentXPos, 4, endingFrame);
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
