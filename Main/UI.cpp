#include "UI.h"

//#######################################################################
// GLOBALS
//#######################################################################

extern ILI9488 display;

extern mydata_t my;
extern programdata_t prog;
// Current location = Düsseldorf
location_t myCurrentLocation{ 0, 51.192560, 6.808518 };

//#######################################################################
// Constructors
//#######################################################################

UI::UI() {}



//#######################################################################
// Functions
//#######################################################################
#pragma region Functions
void UI::init() {
  my.location = myCurrentLocation;
  drawBaseUI();

  addButton(0, UI_Button("+", 410, 40, 60, 120));
  buttons[0].setFillColor(UI_BUTTON_FILL_COLOR);
  buttons[0].setLabelTextSize(3);

  addButton(1, UI_Button("-", 410, 190, 60, 120));
  buttons[1].setFillColor(UI_BUTTON_FILL_COLOR);
  buttons[1].setLabelTextSize(3);

  drawUIButtons();
}

// Shows warning message
void UI::showWarning() {
  display.fillScreen(TFT_BLACK);
  // Centers upcoming text around the middle of the screen
  display.setTextDatum(TC_DATUM);
  display.setTextSize(3);
  display.setTextColor(TFT_RED);

  auto fontheight = display.fontHeight(GFXFF);
  display.drawString("WARNING", TFT_X_CENTER, TFT_Y_CENTER - 2 * fontheight, GFXFF);

  display.setTextSize(2);
  display.setTextColor(TFT_WHITE);
  display.drawString("STRICTLY FOR EXPERIMENTAL USE", TFT_X_CENTER, TFT_Y_CENTER, GFXFF);
  display.drawString("NOT INTENDED FOR REAL NAVIGATION", TFT_X_CENTER, TFT_Y_CENTER + fontheight, GFXFF);
  display.drawString("USE AT YOUR OWN RISK!", TFT_X_CENTER, TFT_Y_CENTER + 2 * fontheight, GFXFF);

  delay(5000);
  display.fillScreen(TFT_BLACK);
}

// Draws the Base UI
void UI::drawBaseUI() {
  display.fillScreen(BLUE);

  // Top bar
  display.fillRect(0, 0, WIDTH, 30, DARKGREY);
  display.drawFastHLine(0, 30, WIDTH, WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.print("TRAFFIC");

  // Restore Default Font
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Draw radarcircle
  display.fillCircle(TFT_X_CENTER, TFT_Y_CENTER, TFT_Y_CENTER - 1, BLACK);

  // Draw range text with nautical miles as units
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(WIDTH - WIDTH / 3 + 20, HEIGHT - HEIGHT / 10);
  display.print(prog.currentRange);
  display.setCursor(WIDTH - WIDTH / 3 + 58, HEIGHT - HEIGHT / 10);
  display.print("Nm");
}

void UI::drawUIButtons() {
  for (auto& button : buttons)
    button.second.draw();
}


void UI::setRange(uint16_t range) {
  // Erase range text
  display.setTextSize(2);
  display.setTextColor(BLUE);
  display.setCursor(WIDTH - WIDTH / 3 + 20, HEIGHT - HEIGHT / 10);
  display.print(prog.currentRange);
  display.setCursor(WIDTH - WIDTH / 3 + 58, HEIGHT - HEIGHT / 10);
  display.print("Nm");

  // Change range
  prog.currentRange = range;

  // Draw updated range text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(WIDTH - WIDTH / 3 + 20, HEIGHT - HEIGHT / 10);
  display.print(prog.currentRange);
  display.setCursor(WIDTH - WIDTH / 3 + 58, HEIGHT - HEIGHT / 10);
  display.print("Nm");
}

// Processes Touchscreen input
uint8_t UI::processInput(point_t touchPoint) {
  for (auto& button : buttons) {
    if (touchPoint == button.second.getRegion()) {
      // Animate button press
      buttons[button.first].momentaryPress();
      // Return ID of pressed button (uint8_t)
      return button.first;
    }
  }
  // if touch input dont match any parameters return 255
  return 255;
}

void UI::addButton(uint8_t id, UI_Button button) {
  buttons.insert({ id, button });
}

#pragma endregion Functions