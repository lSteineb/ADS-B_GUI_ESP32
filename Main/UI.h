#ifndef _GUI_
#define _GUI_

#include <cmath>
#include <string>
#include <sys/stat.h>
#include "Aircraft.h"
#include "UI_Types.h"

#define EARTH_RAD_KM 6371
#define EARTH_RAD_NM 3440
#define RANGE 25

// Current location = Düsseldorf
location_t myloc{ 0, 51.192560, 6.808518 };
program_states_t states;

// draws the Base UI
void drawBaseUI(Display tft) {
  states.currentRange = 25;
  tft.fillScreen(TFT_BLUE);

  // Top Black Bar
  tft.fillRect(0, 0, WIDTH, 30, TFT_DARKGREY);
  tft.drawFastHLine(0, 30, WIDTH, TFT_WHITE);

  // Top bar text
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  // System Name
  tft.setCursor(10, 10);
  tft.print("TRAFFIC");

  // Restore Default Font
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);

  // Draw radarcircle
  tft.drawCircle( TFT_X_CENTER, TFT_Y_CENTER, TFT_Y_CENTER - 1, TFT_BLACK);
  tft.fillCircle(TFT_X_CENTER, TFT_Y_CENTER, TFT_Y_CENTER - 1, TFT_BLACK);
  // Draw current position
  tft.drawCircle(TFT_X_CENTER, TFT_Y_CENTER, 3, TFT_RED);

  // Inner Circle Params
  int degMark = 15;
  int ri = HEIGHT / 4;
  // Draw inner circle
  for (int i = 0; i < 360; i += degMark) {
    float a = radians(i);
    tft.drawPixel(TFT_X_CENTER + cos(a) * ri, TFT_Y_CENTER + sin(a) * ri, TFT_WHITE);
  }

  // Draw range text with nautical miles as units
  int offset = 12;
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(WIDTH - WIDTH / 3 + 18 + offset, HEIGHT - HEIGHT / 10);
  tft.print(RANGE);
  tft.setCursor(WIDTH - WIDTH / 3 + 58, HEIGHT - HEIGHT / 10);
  tft.print("Nm");

  // TODO: Implement aircrafts
  /*
  // Redraw traffic	
	for(auto& aircraft : aircrafts) {
		aircraft.second.draw();
	}
  */
}





#endif