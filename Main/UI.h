#ifndef _GUI_
#define _GUI_

#include <cmath>
#include <string>
#include <sys/stat.h>
#include "Aircraft.h"
#include "UI_Types.h"

#define EARTH_RAD_KM 6371
#define EARTH_RAD_NM 3440
#define RANGE 50

// Current location = Düsseldorf
//location_t myloc{ 0, 51.192560, 6.808518 };
program_states_t states;

extern mydata_t my;
// Current location = Köln
location_t myloc { 0, 50.934449, 6.988244 };
//std::unordered_map<uint8_t, ADESP_UI_Button> buttons;

// draws the Base UI
void drawBaseUI(Display tft) {
  my.location = myloc;
/*
  addButton(3, ADESP_UI_Button("+", 410, 40, 60, 120));
  buttons[3].setType(MOMENTARY);
  buttons[3].setFillColor(UI_BUTTON_FILL_COLOR);
  buttons[3].setLabelTextSize(3);
  buttons[3].assignUI(TRAFFIC_UI);

  addButton(4, ADESP_UI_Button("-", 410, 190, 60, 120));
  buttons[4].setType(MOMENTARY);
  buttons[4].setFillColor(UI_BUTTON_FILL_COLOR);
  buttons[4].setLabelTextSize(3);
  buttons[4].assignUI(TRAFFIC_UI);
*/
  //states.currentRange = 25;
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
  tft.drawCircle(TFT_X_CENTER, TFT_Y_CENTER, TFT_Y_CENTER - 1, TFT_BLACK);
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

/* Calculate cartesian X,Y from LAT & LON */
std::pair<float, float> calcXY(float lat, float lon) {
  std::pair<float, float> result;
  float xF, yF, dF;

  xF = ((radians(lon) - radians(my.location.lon)) * cos((radians(my.location.lat) + radians(lat)) / 2)) * EARTH_RAD_NM;
  yF = (radians(lat) - radians(my.location.lat)) * EARTH_RAD_NM;
  dF = sqrt(xF * xF + yF * yF);

  /* Round and scale to selected range */
  result.first = TFT_X_CENTER + round(xF * TFT_Y_CENTER / 50);
  result.second = TFT_Y_CENTER - round(yF * TFT_Y_CENTER / 50);

  float posDistance = round(dF * TFT_Y_CENTER / 50);

  return result;
}



#endif