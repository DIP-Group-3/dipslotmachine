// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x64 RGB LED matrix.

// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3
#define reservePin 15
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);
void playCombAnimation()
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

void setup()
{
    matrix.begin();
    randomSeed(analogRead(reservePin));
}

void loop()
{
    playRainDropAnimation();
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