#ifndef _GUI_
#define _GUI_

#include <cmath>
#include <string>

#include "Aircraft.h"
#include "UI_Types.h"

#define EARTH_RAD_KM 6371
#define EARTH_RAD_NM 3440
#define RANGE 50

// Current location = Düsseldorf
location_t myloc{ 0, 51.192560, 6.808518 };

extern mydata_t my;

// draws the Base UI
void drawBaseUI(ILI9488 tft) {
  my.location = myloc;

  tft.fillScreen(BLUE);

  // Top bar
  tft.fillRect(0, 0, WIDTH, 30, DARKGREY);
  tft.drawFastHLine(0, 30, WIDTH, WHITE);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(10, 10);
  tft.print("TRAFFIC");

  // Restore Default Font
  tft.setTextSize(1);
  tft.setTextColor(WHITE);

  // Draw radarcircle
  tft.fillCircle(TFT_X_CENTER, TFT_Y_CENTER, TFT_Y_CENTER - 1, BLACK);

  // Draw range text with nautical miles as units
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(WIDTH - WIDTH / 3 + 30, HEIGHT - HEIGHT / 10);
  tft.print(RANGE);
  tft.setCursor(WIDTH - WIDTH / 3 + 58, HEIGHT - HEIGHT / 10);
  tft.print("Nm");
}


#endif