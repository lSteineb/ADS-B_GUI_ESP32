#ifndef Warning
#define Warning

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <TFT_eSPI.h>  // Hardware-specific library
#include <SPI.h>
#include <string>

#define TFT_X_CENTER 240
#define TFT_Y_CENTER 160
#define GFXFF 1

class Display : public TFT_eSPI{
public:
  Display();

  void init();
  void showWarning();
};

#endif