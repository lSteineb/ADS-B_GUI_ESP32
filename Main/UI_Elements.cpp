#include "UI_Elements.h"

//#######################################################################
// GLOBALS
//#######################################################################
extern ILI9488 display;



//#######################################################################
// Constructors
//#######################################################################
#pragma region Constructors

UI_Button::UI_Button(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) {
  x = _x;
  y = _y;
  w = _w;
  h = _h;
  region = { x, x + w, y, y + h };
}

UI_Button::UI_Button(const char* buttonLabel, int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) {
  strcpy(label, buttonLabel);
  x = _x;
  y = _y;
  w = _w;
  h = _h;
  region = { x, x + w, y, y + h };
}

#pragma endregion Constructors



//#######################################################################
// Functions
//#######################################################################
#pragma region Functions

void UI_Button::init() {}

void UI_Button::init(uint8_t textSize) {
  labelTextSize = textSize;
}

void UI_Button::setLabelTextSize(uint8_t _size) {
  labelTextSize = _size;
}

void UI_Button::draw() {
  display.drawRoundRect(x, y, w, h, 10, outlineColor);

  display.setTextSize(labelTextSize);
  display.setTextColor(textColor);

  display.setCursor(x + w / 2 - 8, y + h / 2 - 8);
  display.print(label);
}

void UI_Button::drawPressed() {
  display.drawRoundRect(x, y, w, h, 10, pressedColor);
}

void UI_Button::drawReleased() {
  display.drawRoundRect(x, y, w, h, 10, outlineColor);
}

void UI_Button::momentaryPress() {
  drawPressed();
  delay(debounceDelay / portTICK_PERIOD_MS);
  drawReleased();
}

void UI_Button::setLabel(const char* buttonLabel) {
  strcpy(label, buttonLabel);
}

void UI_Button::setLabel(const char* buttonLabel, uint8_t textSize) {
  strcpy(label, buttonLabel);
  labelTextSize = textSize;
}

void UI_Button::setFillColor(uint16_t _color) {
  fillColor = _color;
}

void UI_Button::setPressedColor(uint16_t _color) {
  pressedColor = _color;
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

screenRegion_t UI_Button::getRegion() {
  return region;
}

#pragma endregion Functions