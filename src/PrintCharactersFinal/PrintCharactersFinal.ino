#include <RGBmatrixPanel.h>

//PIN DEFINITIONS
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE 9   // OUTPUT ENABLE
#define LAT 10 // LATCH SIGNAL MARKS THE END OF A ROW OF DATA
#define A A0   // ROW SELECT: WHICH TWO ROWS CURRENTLY LIT
#define B A1
#define C A2
#define D A3
#define R1 24 // UPPER RGB DATA - TOP HALF OF DISPLAY
#define G1 25
#define B1 26
#define R1 27 // LOWER RGB DATA - BOTTOM HALF OF DISPLAY
#define G2 28
#define B2 29

#define piezoPin 4

//ARRAY OF COMBOS & SELECT, Combinations of all possible Frames (10)
String combo[] = {"EEE", "NBS", "IEM", "ADM", "SCE", "NBS", "SCE", "ADM", "EEE", "SCE"};

//Array to determine scroll speed
//ySpeed, or scroll speed is proportional to weights (20)
float spdWeights[] = {0.6, 0.7, 0.8, 0.8, 0.9, 1.0, 1.1, 1.2, 1.5, 1.5, 1.5, 1.5, 1.5, 1.4, 1.3, 1.2, 1.1, 1.1, 1.0, 1.0, 1};

//construct 64x32 LED MATRIX panel
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

//Global Variables
int numOfFrames = 10;
int numberOfRotations = 20;
float ySpeed = 10; //vertical scrollSpeed
bool isSpinning = true; //spin button to trigger status;
int startingFrame; 
int endingFrame;

void setup()
{
  Serial.begin(9600);
  //Set-up code for matrix
  matrix.begin();
  //set text properties
  matrix.setTextSize(3); // size 1 -> hxw = 7x4. size = 3 -> hxw = (7*3)x(4*3) = 21x12
  matrix.setTextWrap(false);  // Don't wrap at end of line - will do ourselves

  // *MUST SEED using Analog input from UNUSED_PIN. analogRead(**UNUSED_PIN**). Pin15 is used here temporarily
  randomSeed(analogRead(15));
}

void loop()
{

  //Example code on how to implement rolling animation
  //{...before}: spin button is pressed

  
 
  if (isSpinning == true)
  {
    startingFrame = random(300) % numOfFrames; //choose random start frame
    endingFrame = random(300) % numOfFrames;   //choose random end frame

    playAnimation(startingFrame, endingFrame, numberOfRotations);
  }
  isSpinning = false;

  
}


void playAnimation(int startingFrame, int endingFrame, int numberOfRotations)
{
  int currentFrame;
  int currentXPos = 0;
  int currentYPos = 0;
  int startXPos = 1; //todo: missing animation: OXO -> OOX -> X00
  int yPosCenter = 6;
  int yPosTop = -21; //starting pos = outside frame


  //initial conditions
  currentXPos = startXPos;
  currentYPos = yPosCenter;//startYPos;
  currentFrame = startingFrame;

  
  //begin animation...is rolling...for numberOfRotations times
  for (int counter = 0; counter < numberOfRotations;)
  {

    matrix.fillScreen(matrix.Color333(0, 0, 0)); //FILL SCREEN 'black'
    drawFrame(currentXPos, currentYPos, currentFrame, matrix.Color333(255,0,0));
    delay(0);
    currentYPos += round(ySpeed * spdWeights[counter]); //move frame down by scrollSpeed scaled by weights
    
    //check if frame exited matrix
    if (currentYPos >= matrix.height() + 21) //text ht = 21
    {                          
      currentYPos = yPosTop; //wrap around/reset to start Position

      currentFrame = (currentFrame + 1) % numOfFrames; // swap to next frame
      counter++;
      tone(piezoPin, 1000, 100); //beep sfx as frame enters matrix
    }
  }

  //ending animation 
  int endYPos = 6;
  for (; currentYPos <= endYPos;)
  {
    //print ROW of IMAGES
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    currentYPos += ySpeed;
    drawFrame(currentXPos, currentYPos, endingFrame, matrix.Color333(255,0,0));
  }

  jitterAnimation(endingFrame, currentXPos, currentYPos);
}

void jitterAnimation(int endingFrame, int currentXPos, int currentYPos){
  char *frameStr = combo[endingFrame].c_str();
  char * iem = "IEM";
  char * eee = "EEE";

  uint16_t colour;
  
  // JACKPOT
  if (!strcmp(iem, frameStr)) {
    Serial.print("JACKPOT, IEM\n");
    colour = matrix.Color333(0, 255, 245);
  }
  // EEE
  else if (!strcmp(eee, frameStr)) {
    Serial.print("EEE\n");
    colour = matrix.Color333(76, 255, 56);
  // OTHERS
  } else {
    colour = matrix.Color333(0, 254, 0);
  }

    //jitter/shake stop animation
  int yPositionsJitter[] = {10, 4, 10, 4, 6};

  for (int i = 0; i < 5; i++)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    currentYPos = yPositionsJitter[i];
    drawFrame(currentXPos, currentYPos, endingFrame, colour);
    Serial.print(colour);
    tone(piezoPin, 5000, 50);
  
  }

}


void drawFrame(int currentXPos, int currentYPos, int frame, uint16_t color)
{
  // DRAW Text
  uint8_t w = 0;
  uint8_t space = 24;
  // Characterise Selected String, extract String from String Array
  char *str = combo[frame].c_str();

  matrix.setCursor(currentXPos, currentYPos);
  for (w = 0; w < 3; w++)
  { // 3 = number of characters
    //matrix.setTextColor(Wheel(w*8-4));
    matrix.setTextColor(color);
    matrix.print(str[w]);
    matrix.setCursor(space, currentYPos);
    space += space;
  }
}

// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos)
{
  if (WheelPos < 8)
  {
    return matrix.Color333(7 - WheelPos, WheelPos, 0);
  }
  else if (WheelPos < 16)
  {
    WheelPos -= 8;
    return matrix.Color333(0, 7 - WheelPos, WheelPos);
  }
  else
  {
    WheelPos -= 16;
    return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}
