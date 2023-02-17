#ifndef _UI_ELEMENTS_
#define _UI_ELEMENTS_

#include "UI_Types.h"

typedef void (*callback_t)();

class UI_Button {
public:
  // Label
  char label[16];
  uint8_t labelTextSize = 1;

  // Dimensions
  int16_t x, y;
  uint16_t w, h;

  screenRegion_t region;

  uint16_t debounceDelay = 200;
  bool toggled = false;
  callback_t callback = nullptr;
  const GFXfont* font;

  // Colors
  uint16_t fillColor = 0;
  uint16_t outlineColor = WHITE;
  uint16_t pressedColor = BLACK;
  uint16_t textColor = WHITE;

  UI_Button() {}
  UI_Button(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h);
  UI_Button(const char* buttonLabel, int16_t _x, int16_t _y, uint16_t _w, uint16_t _h);
  UI_Button(const char* buttonLabel, int16_t _x, int16_t _y, uint16_t _w, uint16_t _h, uint16_t _color);

  void init();
  void init(uint8_t);


  void setFillColor(uint16_t _color);
  void setPressedColor(uint16_t _color);
  void setCallback(callback_t);

  void draw();

  void setLabelTextSize(uint8_t _size);
  void setLabel(const char*);
  void setLabel(const char*, uint8_t);

  bool toggle();
  void drawPressed();
  void drawReleased();
  void momentaryPress();

  bool isActive();
  void setActive();
  void setInactive();

  void setDebounce(uint16_t);
  void setFont(const GFXfont* _font);

  screenRegion_t getRegion();
private:
};


#endif