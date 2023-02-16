#include "UI_Elements.h"

//#######################################################################
// GLOBALS
//#######################################################################
extern ILI9488 display;



//#######################################################################
// Constructors
//#######################################################################

UI_Button::UI_Button(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) {
  x = _x;
  y = _y;
  w = _w;
  h = _h;
}

UI_Button::UI_Button(const char* buttonLabel, int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) {
  strcpy(label, buttonLabel);
  x = _x;
  y = _y;
  w = _w;
  h = _h;
}



//#######################################################################
// Functions
//#######################################################################

void UI_Button::init() {}

void UI_Button::init(uint8_t textSize) {
  labelTextSize = textSize;
}

void UI_Button::setLabelTextSize(uint8_t _size) {
  labelTextSize = _size;
}

void UI_Button::draw() {
  display.drawRoundRect(x, y, w, h, 10, outlineColor);

  int16_t xT, yT;
  uint16_t wT, hT;

  display.setTextSize(labelTextSize);
  display.setTextColor(textColor);

  display.getTextBounds(label, 0, 0, &xT, &yT, &wT, &hT);
  display.setCursor(x + w / 2 - wT / 2, y + h / 2 - hT / 2);
  display.print(label);
}

void UI_Button::setLabel(const char* buttonLabel) {
  strcpy(label, buttonLabel);
}

void UI_Button::setLabel(const char* buttonLabel, uint8_t textSize) {
  strcpy(label, buttonLabel);
  labelTextSize = textSize;
}

bool UI_Button::toggle() {
  if (toggled)
    toggled = false;
  else
    toggled = true;

  return toggled;
}

bool UI_Button::isActive() {
  return toggled;
}

void UI_Button::setDebounce(uint16_t delayMS) {
  debounceDelay = delayMS;
}
