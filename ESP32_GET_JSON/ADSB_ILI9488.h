#ifndef Warning
#define Warning

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <TFT_eSPI.h>  // Hardware-specific library
#include <SPI.h>
#include <string>

#define TFT_X_CENTER 480 / 2
#define TFT_Y_CENTER 320 / 2
#define GFXFF 1

class Display : public TFT_eSPI{
private:
  //TFT_eSPI tft;
public:
  Display();

  void createWarning();
  void drawWarning();
};

#endif