#include "ILI9488.h"

//#######################################################################
// Contructors
//#######################################################################
ILI9488::ILI9488() {}



//#######################################################################
// Functions
//#######################################################################

// Init
void ILI9488::init() {
  begin();
  setRotation(-1);
}


void ILI9488::touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // Check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // Check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL) {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    } else {
      fs::File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // Calibration data valid
    setTouch(calData);
  } else {
    // Data not valid so recalibrate
    fillScreen(TFT_BLACK);
    setCursor(20, 0);
    setTextFont(2);
    setTextSize(1);
    setTextColor(TFT_WHITE, TFT_BLACK);

    println("Touch corners as indicated");

    setTextFont(1);
    println();

    if (REPEAT_CAL) {
      setTextColor(TFT_RED, TFT_BLACK);
      println("Set REPEAT_CAL to false to stop this running again!");
    }

    calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    setTextColor(TFT_GREEN, TFT_BLACK);
    println("Calibration complete!");

    // Store data
    fs::File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

// Draws a Rhomb
void ILI9488::drawRhomb(int16_t x0, int16_t y0, int16_t size, uint16_t color) {
  drawLine(x0, y0 - size, x0 - size, y0, color);
  drawLine(x0, y0 - size, x0 + size, y0, color);
  drawLine(x0 - size, y0, x0, y0 + size, color);
  drawLine(x0 + size, y0, x0, y0 + size, color);
}

// Erases given text at pos(x,y)
void ILI9488::eraseText(const char *str, uint16_t xPos, uint16_t yPos, uint16_t color) {
  int16_t x, y;
  uint16_t w, h;

  getTextBounds(str, xPos, yPos, &x, &y, &w, &h);
  fillRect(xPos, yPos, w, h, color);
}

// Erases given number at pos(x,y)
void ILI9488::eraseText(int n, uint16_t xPos, uint16_t yPos, uint16_t color) {
  int16_t x, y;
  uint16_t w, h;

  char str[8];
  sprintf(str, "%d", n);
  getTextBounds(str, xPos, yPos, &x, &y, &w, &h);
  fillRect(xPos, yPos, w, h, color);
}

// Gets text bounds
void ILI9488::getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
  uint8_t c;

  *x1 = x;
  *y1 = y;
  *w = *h = 0;

  int16_t minx = WIDTH, miny = WIDTH, maxx = -1, maxy = -1;

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
void ILI9488::charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {
  auto textsize = fontHeight(GFXFF);
  if (c == '\n') {       // Newline?
    *x = 0;              // Reset x to zero,
    *y += textsize * 8;  // advance y one line
    // min/max x/y unchaged -- that waits for next 'normal' character
  } else if (c != '\r') {                                        // Normal char; ignore carriage returns
    if (/*wrap*/ false && ((*x + textsize * 6) > TFT_HEIGHT)) {  // Off right?
      *x = 0;                                                    // Reset x to zero,
      *y += textsize * 8;                                        // advance y one line
    }
    int x2 = *x + textsize * 6 - 1,  // Lower-right pixel of char
      y2 = *y + textsize * 8 - 1;
    if (x2 > *maxx) *maxx = x2;  // Track max x, y
    if (y2 > *maxy) *maxy = y2;
    if (*x < *minx) *minx = *x;  // Track min x, y
    if (*y < *miny) *miny = *y;
    *x += textsize * 6;  // Advance x one char
  }
}
