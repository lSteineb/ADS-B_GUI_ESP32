#ifndef _ILI9488_
#define _ILI9488_

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <string>

#define WIDTH TFT_HEIGHT
#define HEIGHT TFT_WIDTH
#define TFT_X_CENTER WIDTH / 2
#define TFT_Y_CENTER HEIGHT / 2
#define GFXFF 1

class ILI9488 : public TFT_eSPI {
public:
  ILI9488();

  void init();
  void showWarning();

  void eraseText(int, uint16_t, uint16_t, uint16_t);
  void eraseText(const char *, uint16_t, uint16_t, uint16_t);

  void drawRhomb(int16_t, int16_t, int16_t, uint16_t);

  void getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
};

#endif