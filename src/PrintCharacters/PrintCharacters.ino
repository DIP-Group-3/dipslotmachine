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

void setup() {
  matrix.begin();
}

void loop() {
  // Gerenerate Random Index
  int RandIndex = rand() % 10;

  // Characterise Selected String
  char *str = combo[RandIndex].c_str();

  // FILL SCREEN 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // DRAW TEXT
  matrix.setTextSize(3);        // size 1 == 8 pixels high
  matrix.setTextWrap(false);    // Don't wrap at end of line - will do ourselves
  matrix.setCursor(1, 5);       // start at top left, with 8 pixel of spacing

  uint8_t w = 0;                //Color based on position
  uint8_t space = 24;           //Gap between each character

  for (w=0; w<3; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);           // Print Character
    matrix.setCursor(space, 5);     // Position for next character
    space += space;                 // Next x-position
  }
  delay(2000);                      // Time delay for next set of combinations to be printed

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
