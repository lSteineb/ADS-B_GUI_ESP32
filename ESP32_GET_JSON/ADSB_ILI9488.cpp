#include "ADSB_ILI9488.h"

Display::Display() {}

void Display::init() {
  begin();
  setRotation(-1);
}

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
