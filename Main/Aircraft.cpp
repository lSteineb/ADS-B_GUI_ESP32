#include "Aircraft.h"

//#######################################################################
// GLOBALS
//#######################################################################
#define TFT_DRAWABLE (HEIGHT / 2) - AIRCRAFT_SIZE
#define TFT_LBL_DRAWABLE (HEIGHT / 2) - (AIRCRAFT_SIZE + 18)

extern ILI9488 display;
extern mydata_t my;

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

  if (vectorSet)
    getVectorXY();

  if (locationSet)
    draw();
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
  pos.x = TFT_X_CENTER + round(xF * (HEIGHT / 2) / 50);
  pos.y = TFT_Y_CENTER - round(yF * (HEIGHT / 2) / 50);

  posDistance = round(dF * (HEIGHT / 2) / 50);
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

#pragma endregion Getter