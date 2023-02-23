#include "Aircraft.h"

//#######################################################################
// GLOBALS
//#######################################################################
#define TFT_DRAWABLE (HEIGHT / 2) - AIRCRAFT_SIZE
#define TFT_LBL_DRAWABLE (HEIGHT / 2) - (AIRCRAFT_SIZE + 18)

extern ILI9488 display;
extern mydata_t my;
extern programdata_t prog;

static int seconds() {
  return round(millis() / 1000);
}

//#######################################################################
// Contructors
//#######################################################################
#pragma region Constructors

Aircraft::Aircraft() {}

Aircraft::Aircraft(location_t _location) {
  lastSeen = seconds();
  location = _location;
  locationSet = true;

  getXY();
  previousPos.push_back({ pos.x, pos.y });
}

Aircraft::Aircraft(vector_t _vector) {
  lastSeen = seconds();
  vector = _vector;
  vectorSet = true;

  getVectorXY();
}

#pragma endregion Constructors



//#######################################################################
// Functions
//#######################################################################
#pragma region Functions

// Draws aircraft with its current parameters
void Aircraft::draw() {
  // If location not set -> return
  if (!locationSet || pos.x == 0 || pos.y == 0)
    return;

  // If location outside of drawing area -> return
  if (posDistance >= TFT_DRAWABLE)
    return;

  // Draw aircraft
  if (emergency)
    display.drawRhomb(pos.x, pos.y, AIRCRAFT_SIZE, RED);
  else
    display.drawRhomb(pos.x, pos.y, AIRCRAFT_SIZE, WHITE);

  // Draw altitude label in FL format (FL350 = 35000ft)
  lbl = round((float)location.alt / 100);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (posDistance <= TFT_LBL_DRAWABLE) {
    if (vector.vrate >= 100) {
      display.setCursor(pos.x + 5, pos.y + 5);
      display.setTextColor(SKYBLUE);
      display.print(lbl);
    } else if (vector.vrate <= -100) {
      display.setCursor(pos.x + 5, pos.y + 5);
      display.setTextColor(ORANGE);
      display.print(lbl);
    } else {
      display.setCursor(pos.x + 5, pos.y + 5);
      display.setTextColor(WHITE);
      display.print(lbl);
    }
    display.setTextColor(WHITE);
  }

  // Draw emergency label
  if (emergency) {
    if (posDistance <= TFT_LBL_DRAWABLE) {
      display.setCursor(pos.x + 5, pos.y - 10);
      display.setTextColor(RED);
      display.print("EM");
    }
  }

  // If vector outside of drawing area -> return
  if (vectDistance >= TFT_DRAWABLE)
    return;

  // Draw aircraft Vector if set
  if (vectorSet)
    display.drawLine(pos.x, pos.y, vec.x, vec.y, GREEN);
}

// Erases aircraft from display
void Aircraft::erase() {
  // If location set and outside drawing area -> return
  if (locationSet && posDistance >= TFT_DRAWABLE)
    return;

  // Erase aircraft
  display.drawRhomb(pos.x, pos.y, AIRCRAFT_SIZE, BLACK);

  // Erase altitude label
  if (posDistance <= TFT_LBL_DRAWABLE) {
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(pos.x + 5, pos.y + 5);
    display.print(lbl);
  }

  // Erase emergency label
  if (emergency) {
    if (posDistance <= TFT_LBL_DRAWABLE) {
      display.setCursor(pos.x + 5, pos.y - 10);
      display.setTextColor(BLACK);
      display.print("EM");
    }
  }

  // Draw last position to lengthen the trail when selected
  if (selected)
    display.drawCircle(previousPos.back().x, previousPos.back().y, 1, RED);

  // If vector set and outside drawing area -> return
  if (vectorSet && vectDistance >= TFT_DRAWABLE)
    return;

  // Erase vector if set
  if (vectorSet)
    display.drawLine(pos.x, pos.y, vec.x, vec.y, BLACK);
}

// Updates aircraft position on screen by erasing it and redrawing it at the new position
void Aircraft::update() {
  if (locationSet) {
    erase();
    getXY();
  }

  /*
  if (selected)
    selected = false;
  */

  if (vectorSet)
    getVectorXY();

  if (locationSet)
    draw();

  /*
  if (selected)
    displayInformation();
  */
}

// Check if touchpoint is inside aircraft
bool Aircraft::checkCollision(point_t touch_point) {
  return (touch_point.x >= pos.x - AIRCRAFT_SIZE / 2 - 4 && touch_point.x <= pos.x + AIRCRAFT_SIZE / 2 + 4 && touch_point.y >= pos.y - AIRCRAFT_SIZE / 2 - 4 && touch_point.y <= pos.y + AIRCRAFT_SIZE / 2 + 4);
}

// Displays additional aircraft information in the top right of the screen
void Aircraft::displayInformation() {
  try {
    display.fillRect(5, 35, 95, 50, BLUE);
    display.drawRoundRect(5, 35, 95, 50, 10, WHITE);

    display.setCursor(10, 40);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.print("flight:");
    display.println(information.flight);

    display.setCursor(10, 50);
    display.print("hex:");
    display.println(information.icao_hex);

    display.setCursor(10, 60);
    display.print("squawk:");
    display.println(information.squawk);

    display.setCursor(10, 70);
    display.print("category:");
    display.println(information.category);
  } catch (...) {
    Serial.println("ERROR: Unable to retrieve data!");
  }
}

// Draws previous positions of the aircraft as a trail behind it
void Aircraft::drawTrail(uint32_t color) {
  for (auto& point : previousPos) {
    display.drawCircle(point.x, point.y, 2, color);
  }
}

#pragma endregion Functions



//#######################################################################
// Setter
//#######################################################################
#pragma region Setter

// Sets aircraft location_t = {altitude, latitude, longitude}
void Aircraft::setLocation(location_t newLocation) {
  lastSeen = seconds();

  if (newLocation != location) {

    if (locationSet) erase();
    else locationSet = true;

    location = newLocation;

    previousPos.push_back({ pos.x, pos.y });

    getXY();


    if (vectorSet)
      getVectorXY();

    draw();
  }
}

// Sets aircraft vector_t = {speed, heading, vrate}
void Aircraft::setVector(vector_t newVector) {

  lastSeen = seconds();

  if (newVector != vector) {

    if (locationSet)
      erase();

    vectorSet = true;

    vector = newVector;

    getVectorXY();

    if (locationSet)
      draw();
  }
}

// Sets additional information_t = {icao_hex, squawk, flight, category}
void Aircraft::setInformation(information_t newInformation) {

  lastSeen = seconds();

  if (newInformation != information) {
    information = newInformation;
  }

  // 7700 = general emergency
  // 7600 = radio failure
  // 7500 = attempted hijacking/breach of the flightdeck
  if (information.squawk == "7700" || information.squawk == "7600" || information.squawk == "7500")
    emergency = true;
  else
    emergency = false;
}

// Sets if aircraft is selected on touchscreen
void Aircraft::setSelected(bool select) {
  selected = select;
}

#pragma endregion Setter



//#######################################################################
// Getter
//#######################################################################
#pragma region Getter

// Calculates current aircraft position in relation to current position
void Aircraft::getXY() {

  float xF, yF, dF;

  // Calculate cartesian X,Y from LAT & LON
  xF = ((radians(location.lon) - radians(my.location.lon)) * cos((radians(my.location.lat) + radians(location.lat)) / 2)) * EARTH_RAD_NM;
  yF = (radians(location.lat) - radians(my.location.lat)) * EARTH_RAD_NM;
  dF = sqrt(xF * xF + yF * yF);

  // Round and scale to selected range
  pos.x = TFT_X_CENTER + round(xF * (HEIGHT / 2) / prog.currentRange);
  pos.y = TFT_Y_CENTER - round(yF * (HEIGHT / 2) / prog.currentRange);

  posDistance = round(dF * (HEIGHT / 2) / prog.currentRange);
}

// Gets current aircraft vector
void Aircraft::getVectorXY() {

  uint8_t vectorLength = vector.speed / VECTOR_SIZE;

  float vectorX = (cos(radians(vector.heading - 90)) * vectorLength) + pos.x;
  float vectorY = (sin(radians(vector.heading - 90)) * vectorLength) + pos.y;

  vec.x = round(vectorX);
  vec.y = round(vectorY);

  float vdist = sqrt(pow(vec.x - TFT_X_CENTER, 2) + pow(vec.y - TFT_Y_CENTER, 2));
  vectDistance = round(vdist);
}

// Gets the time when the aircraft was last seen
uint16_t Aircraft::getLastSeen(void) {
  return lastSeen;
}

// Gets the Vector of the aircraft
vector_t Aircraft::getVector(void) {
  return vector;
}

// Gets the location of the aircraft
location_t Aircraft::getLocation(void) {
  return location;
}

// Gets the additional aircraft information
information_t Aircraft::getInformation(void) {
  return information;
}

// Gets if aircraft has been selected on the touchscreen
bool Aircraft::getSelected(void) {
  return selected;
}

#pragma endregion Getter