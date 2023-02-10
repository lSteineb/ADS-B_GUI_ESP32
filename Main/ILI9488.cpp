#include "ILI9488.h"

Display tft;

// Constructor
Display::Display() {}

// Init
void Display::init() {
  begin();
  setRotation(-1);
  showWarning();
}

// Shows warning message
void Display::showWarning() {
  fillScreen(TFT_BLACK);

  setTextDatum(TC_DATUM);  // centers upcoming text around the middle of the screen
  setTextSize(3);
  setTextColor(TFT_RED);

  auto fontheight = fontHeight(GFXFF);
  drawString("WARNING", TFT_X_CENTER, TFT_Y_CENTER - 2 * fontheight, GFXFF);

  setTextSize(2);
  setTextColor(TFT_WHITE);
  drawString("STRICTLY FOR EXPERIMENTAL USE", TFT_X_CENTER, TFT_Y_CENTER, GFXFF);
  drawString("NOT INTENDED FOR REAL NAVIGATION", TFT_X_CENTER, TFT_Y_CENTER + fontheight, GFXFF);
  drawString("USE AT YOUR OWN RISK!", TFT_X_CENTER, TFT_Y_CENTER + 2 * fontheight, GFXFF);

  delay(5000);
  fillScreen(TFT_BLACK);
}

// Draws a Rhomb
void Display::drawRhomb(int16_t x0, int16_t y0, int16_t size, uint16_t color) {
  drawLine(x0, 		      y0 - size, 	    x0 - size, 	    y0, 		    color);
	drawLine(x0, 		      y0 - size, 	    x0 + size, 	    y0, 		    color);
	drawLine(x0 - size,     y0, 		        x0, 	    	y0 + size, 	  color);
	drawLine(x0 + size,     y0, 		        x0, 		    y0 + size, 	  color);
}

// Erases given text at pos(x,y)
void Display::eraseText(const char* str, uint16_t xPos, uint16_t yPos, uint16_t color) {
	int16_t x,y;
	uint16_t w,h;
	
	getTextBounds(str, xPos, yPos, &x, &y, &w, &h);
	fillRect(xPos, yPos, w, h, color);
}

// Erases given number at pos(x,y)
void Display::eraseText(int n, uint16_t xPos, uint16_t yPos, uint16_t color) {
	int16_t x,y;
	uint16_t w,h;

	char str[8];
	sprintf(str, "%d", n);
	getTextBounds(str, xPos, yPos, &x, &y, &w, &h);
	fillRect(xPos, yPos, w, h, color);
}

// Gets text bounds
void Display::getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
	uint8_t c; // Current character

	*x1 = x;
	*y1 = y;
	*w = *h = 0;


	int16_t minx = TFT_HEIGHT, miny = TFT_HEIGHT, maxx = -1, maxy = -1;

	while ((c = *str++))
		charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

	if (maxx >= minx) {
		*x1 = minx;
		*w = maxx - minx + 1;
	}
	if (maxy >= miny) {
		*y1 = miny;
		*h = maxy - miny + 1;
	}
}

// Gets character bounds
void Display::charBounds(char c, int16_t *x, int16_t *y,	int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {
  auto textsize = fontHeight(GFXFF);
  if (c == '\n') {                     // Newline?
    *x = 0;                        // Reset x to zero,
    *y += textsize * 8;             // advance y one line
    // min/max x/y unchaged -- that waits for next 'normal' character
  }
  else if (c != '\r') {  // Normal char; ignore carriage returns
    if (/*wrap*/ false && ((*x + textsize * 6) > TFT_HEIGHT)) { // Off right?
      *x = 0;                    // Reset x to zero,
      *y += textsize * 8;         // advance y one line
    }
    int x2 = *x + textsize * 6 - 1, // Lower-right pixel of char
      y2 = *y + textsize * 8 - 1;
    if (x2 > *maxx) *maxx = x2;      // Track max x, y
    if (y2 > *maxy) *maxy = y2;
    if (*x < *minx) *minx = *x;      // Track min x, y
    if (*y < *miny) *miny = *y;
    *x += textsize * 6;             // Advance x one char
  }
}

