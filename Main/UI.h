#ifndef _GUI_
#define _GUI_

#include <cmath>
#include <string>
#include <unordered_map>

#include "Aircraft.h"
#include "UI_Types.h"
#include "UI_Elements.h"

#define EARTH_RAD_KM 6371
#define EARTH_RAD_NM 3440


class UI {
public:
  std::unordered_map<uint8_t, UI_Button> buttons;

  UI();

  void init();
  void drawBaseUI();
  void drawUIButtons();
  void showWarning();
  void setRange(uint16_t);

  uint8_t processInput(point_t);
  void addButton(uint8_t, UI_Button);

};



#endif