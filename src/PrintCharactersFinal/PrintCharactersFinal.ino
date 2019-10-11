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
float spdWeights[] = {0.6, 0.7, 0.8, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.4, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5};

//construct 64x32 LED MATRIX panel
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

//Global Variables
int numOfFrames = 10;
int numberOfRotations = 20;
//float ySpeed = 10; //vertical scrollSpeed
const float ySpeedsConstant[] = {10, 7, 8}; //original ySpeed
float currentYSpeeds[] = {0, 0, 0}; 

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
  startingFrame = random(300) % numOfFrames; //choose random start frame
  endingFrame = random(300) % numOfFrames;   //choose random end frame

  if (isSpinning == true)
  {
    displayStartingFrame(startingFrame);
    delay(2000);
    playAnimation(startingFrame, endingFrame, numberOfRotations);
  }
  isSpinning = false;
}

//todo: fix logic
void displayStartingFrame(int startingFrame)
{
  //initialise initial conditions:
  //lazy... hardcoded:
  int xPos[] = {1, 24, 47};
  int yPos[] = {6, 6, 6};
  int currentCharacter[] = {0, 0, 0};
  for (int i = 0; i < 3; i++)
  {
    //initial char to display
    currentCharacter[i] = startingFrame;
  }

  for (int i = 0; i < 3; i++)
  {
    char charToDraw = extractCharFromFrameList(currentCharacter[i], i);
    drawCharacter(xPos[i], yPos[i], charToDraw, matrix.Color333(255, 0, 0));
  }
}

void playAnimation(int startingFrame, int endingFrame, int numberOfRotations)
{
  // currentCharacter[] rationale explained:
  // This array stores 3 pointers that points to each character in a frame.
  // This pointers keep track of what character to display according to the following data structure:
  // Define a frame to be "EEE", or "SCE". These frames are stored in the array combo[]
  // Eg. combo[] contains {"EEE", "SCE", "IEM"...}
  // Index 0 keeps track of the 1st character in frame, index 1 keeps track of 2nd character, index 2 keeps track of 3rd character
  // Eg. for the frame "SCE" at combo[1],
  // currentCharacter[0] = 1 points at 'S', currentCharacter[1] = 1: 'C',currentCharacter[2] = 1: 'E'.
  // For the frame "EEE" and combo[0],
  // currentCharacter[0] = 0: 'E', currentCharacter[1] = 0: 'E', currentCharacter[2] = 0: 'E'
  int currentCharacter[] = {0, 0, 0};

  int currentXPositions[] = {1, 24, 47}; //xPos for character 1, 2 & 3 respectively
  int currentYPositions[] = {0, 0, 0};   //yPos for character 1, 2 & 3 respectively

  int startXPos = 1;
  int yPosCenter = 6;
  int yPosTop = -21; //yPos where character is outside of matrix

  // initialise initial conditions
  for (int i = 0; i < 3; i++)
  {
    // initial yPos of each character of each cylinder = center of matrix
    currentYPositions[i] = yPosCenter;
    //initial char to display
    currentCharacter[i] = startingFrame;
    currentYSpeeds[i] = ySpeedsConstant[i]; 
  }

  //begin animation...is rolling...for numberOfRotations times
  int slowestMovingCharIndex;
  for (int counter = 0; counter < numberOfRotations;)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0)); //FILL SCREEN 'black'
    slowestMovingCharIndex = minimum(currentYSpeeds, 3);

    //DRAW CHARACTERS
    //for cylinder/character 1, 2, 3 respectively, draw character
    for (int i = 0; i < 3; i++)
    {
      //extract character from character pointer
      char characterToPrint = extractCharFromFrameList(currentCharacter[i], i); //row, col respectively
      drawCharacter(currentXPositions[i], currentYPositions[i], characterToPrint, matrix.Color333(255, 0, 0));
    }
    delay(0);

    //UPDATE COORDINATES
    //for each character in cylinder 1, 2, 3, move yCoordinate of character by its respective ySpeed
    for (int i = 0; i < 3; i++)
    {
      //move character down by scrollSpeed scaled by weights
      currentYPositions[i] += round(currentYSpeeds[i] * spdWeights[counter]);
      //check if current character has exited matrix
      if (currentYPositions[i] >= matrix.height() + 21) //text ht = 21
      {
        currentYPositions[i] = yPosTop; //wrap character around/reset to start Position

        //go to next character in column/cylinder
        currentCharacter[i] = (currentCharacter[i] + 1) % numOfFrames;

        if (slowestMovingCharIndex == i)
        { //counter value limited by slowest moving character
          counter++;
        }
      }
    }
  }

  //PRINT ENDING FRAME
  for (int i = 0; i < 3; i++)
  {
    currentCharacter[i] = endingFrame; //set each char pointer to point at endingFrame
  }

  //ending animation: draw ending frame
  //the following code aims to pull character to the center of the matrix
  //idea: frameArrivedAtCenter[] keeps track of whether the characters that has reached middle of matrix
  //if it has reached the center, oscillate about the center until all 3 characters reached the center
  //this code will be significant if speed diff is huge for each character
  bool frameArrivedAtCenter[] = {false, false, false}; //for cylinders 1, 2, 3 respectively
  uint16_t colour;
  while (true)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    for (int i = 0; i < 3; i++) //for each character of each cylinder,
    {
      //DRAW CHARACTERS
      char characterToPrint = extractCharFromFrameList(currentCharacter[i], i); //row, col respectively
      drawCharacter(currentXPositions[i], currentYPositions[i], characterToPrint, colour);

      //UPDATE COORDINATES
      if (currentYPositions[i] <= yPosCenter) //if character hasn't reached middle of matrix
      {
        currentYPositions[i] += currentYSpeeds[i]; //move character down by its respective speed in ySpeed[]
      }
      else //this character has arrived/passed at/the center. Proceed to oscillate about center
      {
        //update character status: char has reached or passed the center
        frameArrivedAtCenter[i] = true;
        if (frameArrivedAtCenter[i] == true)
        {
          currentYPositions[i] = 4; //force to (matrix center - 2) position. Position hardcoded, need to draw to visualise.
          currentYSpeeds[i] = 2;    //for oscillating logic. Char will osscilate about center of matrix w amplitude = 2 (+/- 2 about center)
        }
        currentYPositions[i] -= currentYSpeeds[i];
      }

      //CHOOSE COLOUR OF CHARACTERS TO PRINT
      if (frameArrivedAtCenter[i] == true) //if char has passed the center, print winning condition colour
      {
        colour = getColourToPrintBasedOnEndingFrame(endingFrame);
      }
      else //else, use original colour
      {
        colour = matrix.Color333(255, 0, 0);
      }
    }

    if (frameArrivedAtCenter[0] && frameArrivedAtCenter[1] && frameArrivedAtCenter[2]) //if all frames has arrived at the center
      break;                                                                           //exit
  }

  //final jitter animation to bring frames to a stop
  oscillateWithDecreasingEnergyAnimation(currentXPositions, currentYPositions, currentCharacter, endingFrame);
}
int minimum(float array[], int size) //return index of min value in array
{
  float min = array[0];
  int index;
  for (int i = 1; i < size; i++)
  {
    if (min > array[i])
    {
      min = array[i];
      index = i;
    }
  }
  return index;
}

uint16_t getColourToPrintBasedOnEndingFrame(int endingFrame)
{
  uint16_t colour;
  String frameToPrint = combo[endingFrame];
  //set character colour
  if (frameToPrint.equalsIgnoreCase("IEM"))
  { //JACKPOT
    colour = matrix.Color333(0, 255, 245);
  }
  else if (frameToPrint.equalsIgnoreCase("EEE"))
  { //EEE
    colour = matrix.Color333(76, 255, 56);
  }
  else
  { //OTHERS
    colour = matrix.Color333(76, 255, 56);
  }

  return colour;
}

void oscillateWithDecreasingEnergyAnimation(int currentXPositions[], int currentYPositions[], int currentCharacter[], int endingFrame) //(int endingFrame, int currentXPos, int[] currentYPositions)
{

  //String frameToPrint = combo[endingFrame];
  uint16_t colour = getColourToPrintBasedOnEndingFrame(endingFrame);

  //oscillateAnimation
  int yPositionsOscillate[] = {8, 4, 8, 4, 8, 6};
  for (int j = 0; j < 6; j++)
  {
    matrix.fillScreen(matrix.Color333(0, 0, 0)); //FILL SCREEN 'black'
    for (int i = 0; i < 3; i++)
    { //for each character in cylinder 1, 2, 3 respectively
      //drawFrame
      char charToDraw = extractCharFromFrameList(currentCharacter[i], i);
      drawCharacter(currentXPositions[i], yPositionsOscillate[j], charToDraw, colour);
      //buzz SFX
      tone(piezoPin, 5000, 50);
    }
  }
}

char extractCharFromFrameList(int rowNumber, int colNumber)
{
  return combo[rowNumber].charAt(colNumber); //eg. combo[1] returns "NBS". combo[1].at(2) returns "B".
}

void drawCharacter(int xPos, int yPos, char characterToPrint, uint16_t color)
{
  // DRAW Text
  uint8_t w = 0;
  matrix.setTextColor(color);
  matrix.setCursor(xPos, yPos);
  matrix.print(characterToPrint);
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
