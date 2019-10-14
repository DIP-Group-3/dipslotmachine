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

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);
void playRainDropAnimation()
{
    //VARIABLES
    int currentXStarts[];
    int currentYStarts[];
    int currentYEnds[];
    int ySpeeds[];
    int lineLength;
    int numOfRainDrops = 32;

    //initialise VARIABLES, create 32 rain drops
    for (int i = 0; i < numOfRainDrops; i++)
    {
        currentXStarts[i] = i * 2; //print raindrop every even interval
        currentYStarts[i] = 0;
        lineLength = rand(5, 9); //generate length between 5&9 inclusive? todo: check
        currentYEnds[i] = currentYStarts[i] + lineLength;
        ySpeeds[i] = rand(5, 10);
    }

    //DRAW
    for (int i = 0; i < numOfRainDrops; i++)
    {
        //drawline
        drawLine(currentXStarts[i], currentYStarts[i], currentYEnds[i])
    }
    //MOVE COORDINATES
    for (int i = 0; i < numOfRainDrops; i++)
    {
        currentYStarts[i] += ySpeeds[i];
        currentYEnds[i] += ySpeeds[i];

        if (currentYStarts > matrix.height())
        { //reset line pos
            currentYStarts[i] = 0 - lineLength;
            currentYEnds[i] = 0;
        }
    }
}

void drawLine(int currentXStart, int currentYStart, int currentYEnds)
{
    for (int i = yPos, i < lineLength, i++)
    { //todo: lineLength or lineLength+1?
        uint16_t colour = Wheel(i + currentXStart);
        int yPos = i;
        matrix.drawPixel(currentXStart, yPos, colour);
    }
}

void setup()
{
}

void loop()
{
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