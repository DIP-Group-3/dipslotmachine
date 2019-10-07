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
//float ySpeed = 10; //vertical scrollSpeed
float ySpeeds[] = {10, 5, 6}; //ySpeed for cylinder 1, 2 and 3 respectively

bool isSpinning = true; //spin button to trigger status;
int startingFrame;
int endingFrame;

void setup()
{
  Serial.begin(9600);
  //Set-up code for matrix
  matrix.begin();
  //set text properties
  matrix.setTextSize(3);     // size 1 -> hxw = 7x4. size = 3 -> hxw = (7*3)x(4*3) = 21x12
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

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
  //int currentFrame;
  //int currentXPos = 0;
  
  //currentCharacter[] rationale explained:
  //This array stores 3 pointers that points to each character in a frame. 
  //This pointers keep track of what character to display according to the following data structure:
  //Define a frame to be "EEE", or "SCE". These frames are stored in the array combo[]
  //Eg. combo[] contains {"EEE", "SCE", "IEM"...}
  //Index 0 keeps track of the 1st character in frame, index 1 keeps track of 2nd character, index 2 keeps track of 3rd character
  //Eg. for the frame "SCE" at combo[1],
  //currentCharacter[0] = 1 points at 'S', currentCharacter[1] = 1: 'C',currentCharacter[2] = 1: 'E'.
  //For the frame "EEE" and combo[0],
  //currentCharacter[0] = 0: 'E', currentCharacter[1] = 0: 'E', currentCharacter[2] = 0: 'E'
  int[] currentCharacter = {0,0,0}; 

  int currentXPositions[] = {0, 0, 0,} //xPos for character 1, 2 & 3 respectively
  //int currentYPos = 0;
  int currentYPositions[] = {0, 0, 0}; //yPos for character 1, 2 & 3 respectively

  int startXPos = 1; 
  int yPosCenter = 6;
  int yPosTop = -21; //yPos where character is outside of matrix

  //initialise initial conditions
  //currentXPos = startXPos;
  //currentFrame = startingFrame;
  for (int i = 0; i < 3; i++)
  {
    //initial yPos of each character of each cylinder = center of matrix
    currentYPositions[i] = yPosCenter;
    //xPos of each character of each character of cylinder 1, 2, 3 = startXPos, startXPos + 20, startXPos + 20*2
    currentXPositions[i] = startXPos + 20*i;

    //initial frame to display
    currentCharacter[i] = startingFrame;
  }

  //begin animation...is rolling...for numberOfRotations times
  for (int counter = 0; counter < numberOfRotations;)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0)); //FILL SCREEN 'black'
    //print all characters
    for (int i = 0; i<3; i++){
      drawCharacter(currentXPositions[i], currentYPositions[i], currentFrame, i, matrix.Color333(255, 0, 0));
    }
    delay(0);

    
    //for each character in cylinder 1, 2, 3, move yCoordinate of character by its respective ySpeed 
    for (int i = 0; i < 3; i++)
    {
      //move character down by scrollSpeed scaled by weights
      currentYPositions[i] += round(ySpeeds[i] * spdWeights[counter]);
      //check if current character has exited matrix
      if (currentYPositions[i] >= matrix.height() + 21) //text ht = 21
      {
        currentYPositions[i] = yPosTop; //wrap character around/reset to start Position
        counter++;
        tone(piezoPin, 1000, 100); //beep sfx as character exits matrix
        currentCharacter[i] = (currentCharacter[i] + 1)%numOfFrames;
      }

      //currentFrame = (currentFrame + 1) % numOfFrames; // swap to next frame
    }
    
  }

  //todo: check logic
  //ending animation, for each character of each cylinder, pull it down to the center Yposition

  /*
  for (int i = 0; i < 3; i++)
  {
    int endYPos = 6;
    while (currentYPositions[i] <= endYPos)
    {
      //print ROW of IMAGES
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      currentYPositions[i] += ySpeeds[i];
      drawCharacter(currentXPos, currentYPositions, endingFrame, matrix.Color333(255, 0, 0));
    }
  } */

  //todo: refactor and test code
  //ending animation
  //the following code aims to pull character to the center of the matrix
  //idea: counter keeps track of the number of characters that has reached endYPos (middle of matrix)
  //stop pulling the a char of a cylinder down if all 3 characters has reached the middle
  //int counter = 0;
  bool[] frameArrivedAtCenter = {false, false, false} //for cylinders 1, 2, 3 respectively
  while (true))
  {
    for (int i = 0; i < 3; i++) //for each character of each cylinder,
    {
      if (currentYPositions[i] <= endYPos) //if character hasn't reached middle (endYPos) of matrix
      {
        matrix.fillScreen(matrix.Color333(0, 0, 0));
        currentYPositions[i] += ySpeeds[i]; //move character down by its respective speed in ySpeed[]
        //print that character
        drawCharacter(currentXPositions, currentYPositions, endingFrame, matrix.Color333(255, 0, 0));
      }
      else //*PROBLEM: if char isn't moving, any visual artifacts?
        //else, character has reached middle of matrix, don't move it down futher 
        frameArrivedAtCenter[i] = true; //++ to number of characters that has reached the center
    }
    if (frameArrivedAtCenter[0] && frameArrivedAtCenter[1] && frameArrivedAtCenter[2])
      break;
  }

  //todo: fix logic bugs
  //jitter shake animation for each character in each cylinder
  for (int i = 0; i < 3; i++)
  {
    jitterAnimation(endingFrame, currentXPos, currentYPositions); //todo: fix referencing bugs
  }
}

//todo: fix logic bugs
void jitterAnimation(int endingFrame, int currentXPos, int[] currentYPositions)
{
  char *frameStr = combo[endingFrame].c_str();
  char *iem = "IEM";
  char *eee = "EEE";

  uint16_t colour;

  // Choose final frame colour
  // JACKPOT
  if (!strcmp(iem, frameStr))
  {
    Serial.print("JACKPOT, IEM\n");
    colour = matrix.Color333(0, 255, 245);
  }
  // EEE
  else if (!strcmp(eee, frameStr))
  {
    Serial.print("EEE\n");
    colour = matrix.Color333(76, 255, 56);
    // OTHERS
  }
  else
  {
    colour = matrix.Color333(0, 254, 0);
  }

  //jitter/shake stop animation
  int yPositionsJitter[] = {10, 4, 10, 4, 6};

  for (int i = 0; i < 5; i++)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    currentYPositions[i] = yPositionsJitter[i];
    drawCharacter(currentXPositions[i], currentYPositions[i], endingFrame, colour);
    Serial.print(colour);
    tone(piezoPin, 5000, 50);
  }
}

//todo:
//extract character from combo, given character pointer value
char getChar (int cylinderNumber, zzz){
  return combo[cylinderNumber]
}

//todo: test logic
void drawCharacter(int xPos, int yPos, int frame, int cylinderNumber, uint16_t color)
{
  // DRAW Text
  uint8_t w = 0;
  //uint8_t space = 24; //todo: fix spacing logic

  // Characterise Selected String, extract String from String Array
  char *str = combo[frame].c_str();
  // extract character to print frome frame 
  char characterToPrint = str[cylinderNumber];
  matrix.setCursor(xPos, yPos);
  matrix.print(characterToPrint);

  /*
  matrix.setCursor(currentXPos, currentYPos);

  for (w = 0; w < 3; w++) 
  {                       // 3 = number of characters
    //matrix.setTextColor(Wheel(w*8-4));
    matrix.setTextColor(color);
    matrix.print(str[w]);
    matrix.setCursor(space, currentYPositions[w]);
    space += space;
  }
  */
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
