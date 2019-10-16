#include <RGBmatrixPanel.h>

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
#define reservePin 15
#define pi 3.1415926535897932384626433832795

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

// For Waterfall Animation
int waterfallColumns[32];
int waterfallStartingPos[32];
int waterfallRotations = 40;
uint16_t waterfallColor1 = matrix.Color333(0, 210, 84);
uint16_t waterfallColor2 = matrix.Color333(0, 202, 202);
uint16_t waterfallColor3 = matrix.Color333(161, 0, 202);

// For Radition Animation
int radiationRotations = 20;
uint16_t radiationColors[4] = {matrix.Color333(213, 0, 64), matrix.Color333(206, 0, 185),
                               matrix.Color333(161, 0, 202), matrix.Color333(99, 0, 198)};

// For Triangle Spinning Animation
int triangleRadius = 5;
int triangleNumberofRotations = 10;
uint16_t triangleColors[3] = {matrix.Color333(236, 91, 212), matrix.Color333(232, 81, 132),
                              matrix.Color333(228, 98, 72)};

void setup()
{
  // put your setup code here, to run once:
  matrix.begin();
  randomSeed(analogRead(reservePin));
  waterfall();
  radiation();  // Tedious but gaurantee will work
  radiation1(); // Simple but not sure if it will cause any problem
  triangleSpinning();
  sadFace();
  comb();
}

void loop()
{
  // put your main code here, to run repeatedly:
}

// ANIMATION 1: WATERFALL
void waterfall()
{
  // Generate waterfall columns             This gives the X0, X1
  for (int i = 0; i < 32; i++)
  {
    waterfallColumns[i] = (2 * i + 1);
  }
  // Generate waterfall starting positions  This gives the Y0
  for (int i = 0; i < 32; i++)
  {
    waterfallStartingPos[i] = random(0, 31);
  }
  // Rolling animation
  for (int i = 0; i < waterfallRotations; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      // Get the starting and ending pos
      int yStart = waterfallStartingPos[j] + i;
      int yEnd = yStart - 32; // 32 is the length of each waterfall
      if (yEnd < 0 and yStart < 31)
      {
        yEnd = 0;
      }
      else if (yStart > 31)
      {
        yStart = 31;
      }
      else if (yEnd > 31)
      {
        yStart = yStart - 64;
        yEnd = 0;
      }
      // Changing color
      uint16_t currColor;
      int section1 = 6 + sin(j);
      int section2 = 12 + sin(j);
      if (yStart < section1)
      {
        matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], yEnd, waterfallColor1);
      }
      else if (yStart < section2)
      {
        matrix.drawLine(waterfallColumns[j], section1 - 1, waterfallColumns[j], yEnd, waterfallColor1);
        matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], section1, waterfallColor2);
      }
      else
      {
        if (yEnd < section1)
        {
          matrix.drawLine(waterfallColumns[j], section1 - 1, waterfallColumns[j], yEnd, waterfallColor1);
          matrix.drawLine(waterfallColumns[j], section2 - 1, waterfallColumns[j], section1, waterfallColor2);
          matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], section2, waterfallColor3);
        }
        else if (yEnd < section2)
        {
          matrix.drawLine(waterfallColumns[j], section2 - 1, waterfallColumns[j], yEnd, waterfallColor2);
          matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], section2, waterfallColor3);
        }
        else
        {
          matrix.drawLine(waterfallColumns[j], yStart, waterfallColumns[j], yEnd, waterfallColor3);
        }
      }
      // Serial.print();
    }
    Serial.print('\n');
    delay(0);
  }
}

// ANIMATION 2: RADIATION
void radiation()
{
  for (int i = 0; i < radiationRotations; i++)
  {
    for (int x = 0; x < 64; x++)
    {
      for (int y = 0; y < 32; y++)
      {
        int d = (int)sqrt((x - 15) ^ 2 + (y - 31) ^ 2);
        if (d <= 5)
        {
          matrix.drawPixel(x, y, radiationColors[(0 + i) % 4]);
        }
        else if (d <= 10)
        {
          matrix.drawPixel(x, y, radiationColors[(1 + i) % 4]);
        }
        else if (d <= 15)
        {
          matrix.drawPixel(x, y, radiationColors[(2 + i) % 4]);
        }
        else
        {
          matrix.drawPixel(x, y, radiationColors[(3 + i) % 4]);
        }
      }
    }
    delay(0);
  }
}

// ANIMATION 2 (1): RADIATION
void radiation1()
{
  for (int i = 0; i < radiationRotations; i++)
  {
    // section 1 circle
    matrix.fillCircle(15, 31, 5, radiationColors[(0 + i) % 4]);

    // section 2 circle
    for (int r = 6; r < 11; r++)
    {
      matrix.drawCircle(15, 31, r, radiationColors[(1 + i) % 4]);
    }

    // section 3 circle
    for (int r = 11; r < 16; r++)
    {
      matrix.drawCircle(15, 31, r, radiationColors[(2 + i) % 4]);
    }

    // section 4 circle
    for (int r = 16; r < 32; r++)
    {
      matrix.drawCircle(15, 31, r, radiationColors[(3 + i) % 4]);
    }
  }
}

// ANIMATION 3: TRIANGLE SPINNING
void triangleSpinning()
{
  for (int i = 0; i < triangleNumberofRotations; i++)
  {
    for (int angle = 0; angle < 360; angle += 5)
    {
      int x1 = triangleRadius * cos(angle * (pi / 180));
      int y1 = triangleRadius * sin(angle * (pi / 180));

      int x2 = triangleRadius * cos((angle + 120) * (pi / 180));
      int y2 = triangleRadius * sin((angle + 120) * (pi / 180));

      int x3 = triangleRadius * cos((angle + 240) * (pi / 180));
      int y3 = triangleRadius * sin((angle + 240) * (pi / 180));

      uint16_t color;
      if (triangleNumberofRotations < 3)
      {
        color = triangleColors[0];
      }
      else if (triangleNumberofRotations < 6)
      {
        color = triangleColors[1];
      }
      else
      {
        color = triangleColors[2];
      }
      matrix.drawLine(x1, y1, x2, y2, color);
      matrix.drawLine(x2, y2, x3, y3, color);
      matrix.drawLine(x3, y3, x1, y1, color);
    }
  }
}

// ANIMATION 4: FAIL SAD FACE 
void sadFace(){
   drawFace(0 + 10, 0 + 10); 
   drawFace(31 - 10, 31 - 10);
   drawFace(31 + 10, 0 + 10);
   drawFace(63 - 10, 31 - 10);
}

void drawFace(int x, int y){
  uint16_t faceColor = matrix.Color333(256,256,256);
  matrix.drawCircle(x, y, 10, faceColor);      // Face outline
  matrix.fillCircle(x-3, y-3, 2, faceColor);   // Left eye 
  matrix.fillCircle(x+3, y+3, 2, faceColor);   // Right eye 
  for(int angle = 0; angle < 180; angle += 5){ // Mouth
    int i = x + 4 * cos(angle * (pi / 180));
    int j = (y + 7) + 4 * sin(angle * (pi / 180));
    matrix.drawPixel(i, j, faceColor);
  }
}

// ANIMATION 4: COMB 
void comb()
{
    //VARIABLES - Raindrop properties
    //common parameters
    int minLineLength = 5;
    int maxLineLength = 8;
    int lineLengths[32];
    int ySpeed = 10;
    int numOfRainDrops = 32;

    //falling Rectangles parameters
    int fallingRectXStarts[numOfRainDrops];
    int fallingRectYHeads[numOfRainDrops];
    int fallingRectYEnds[numOfRainDrops];

    //rising rectangles parameters
    int risingRectXStarts[numOfRainDrops];
    int risingRectYHeads[numOfRainDrops];
    int risingRectYEnds[numOfRainDrops];

    //CYLINDER/PANEL PROPERTY
    int rectHeight = matrix.height(); //32-1;
    int rectWidth = 64 - 1;

    //initialise VARIABLES, create 32 rain drops
    for (int i = 0; i < numOfRainDrops; i++)
    {
        //XPOS of rects
        fallingRectXStarts[i] = i * 2;    //print raindrop every even interval
        risingRectXStarts[i] = i * 2 + 1; //every odd interval

        lineLengths[i] = random(minLineLength, maxLineLength); //generate length between 5&9 inclusive? todo: check

        //YPos of rects
        fallingRectYHeads[i] = 0 - lineLengths[i] * 2;
        fallingRectYEnds[i] = fallingRectYHeads[i] + lineLengths[i];

        risingRectYEnds[i] = rectHeight + 2 * lineLengths[i];
        risingRectYHeads[i] = risingRectYEnds[i] - lineLengths[i];
    }

    //DRAW ALL RAINDROPS/RECTANGLES
    for (int i = 0; i < numOfRainDrops; i++)
    {
        //draw falling rect
        drawLine(fallingRectXStarts[i], fallingRectYHeads[i], fallingRectYEnds[i]);
        //draw rising rect
        drawLine(risingRectXStarts[i], risingRectYHeads[i], risingRectYEnds[i]);
    }
    int longestLineIndex = minimum(lineLengths, numOfRainDrops);

    //MOVE COORDINATES
    bool longestLineInMatrix = true;
    do
    {
        for (int i = 0; i < numOfRainDrops; i++)
        {
            //move falling rect
            fallingRectYHeads[i] += ySpeed;
            fallingRectYEnds[i] += ySpeed;
            //move rising rect
            risingRectYHeads[i] -= ySpeed;
            risingRectYEnds[i] -= ySpeed;

            if (fallingRectYHeads[longestLineIndex] > matrix.height())
            {
                longestLineInMatrix = false;
            }
        }
    } while (longestLineInMatrix == true);
}
int minimum(int array[], int size) //return index of min value in array
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
void drawLine(int xStart, int yStart, int length)
{
    for (int i = yStart; i < length; i++)
    { //todo: length or length+1?
        uint16_t colour = Wheel((i + xStart) % 24);
        int currentYPos = i;
        matrix.drawPixel(xStart, currentYPos, colour);
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
        return matrix.Color333(WheelPos, 0, 7 - WheelPos);
    }
}