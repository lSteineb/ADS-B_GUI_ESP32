#include "Aircraft.h"

//#######################################################################
// GLOBALS
//#######################################################################
#define TFT_DRAWABLE ( TFT_WIDTH / 2 ) - AIRCRAFT_SIZE
#define TFT_LBL_DRAWABLE ( TFT_WIDTH / 2 ) - ( AIRCRAFT_SIZE + 15 )

extern Display tft;
extern program_states_t states;
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
  draw();
}

#pragma endregion Constructors




//#######################################################################
// Functions
//#######################################################################
#pragma region Functions
// Draws aircraft with its current parameters
void Aircraft::draw() {
  // If location not set >> return
  if (!locationSet || pos.x == 0 || pos.y == 0)
    return;

  // If location outside of drawing area >> return
  if (posDistance >= TFT_DRAWABLE)
    return;

  // Draw aircraft by type
  if (type == ADSB) tft.drawRhomb(pos.x, pos.y, AIRCRAFT_SIZE, WHITE);
  else if (type == MLAT) tft.drawCircle(pos.x, pos.y, AIRCRAFT_SIZE - 1, WHITE);
  else if (type == TISB) tft.fillRect(pos.x - AIRCRAFT_SIZE / 2, pos.y - AIRCRAFT_SIZE / 2, AIRCRAFT_SIZE, AIRCRAFT_SIZE, GREY);
  else
    ;  // pass

  // If vector outside of drawing area >> return
  if (vectDistance >= TFT_DRAWABLE)
    return;

  // Draw aircraft Vector if set
  if (vectorSet) tft.drawLine(pos.x, pos.y, vec.x, vec.y, GREEN);

  // Draw Altitude Label in FL format
  lbl = round((float)location.alt / 100);
  tft.setTextSize(1);
  tft.setTextColor(WHITE);

  if (posDistance <= TFT_LBL_DRAWABLE) {
    if (vectorSet) {
      if (vector.vrate >= 100) {
        tft.fillCircle(pos.x + 12, pos.y, 2, SKYBLUE);
        tft.setCursor(pos.x + 5, pos.y + 5);
        tft.setTextColor(WHITE);
        tft.print(lbl);
      } else if (vector.vrate <= -100) {
        tft.fillCircle(pos.x + 12, pos.y, 2, ORANGE);
        tft.setCursor(pos.x + 5, pos.y + 5);
        tft.setTextColor(WHITE);
        tft.print(lbl);
      } else {
        tft.setCursor(pos.x + 5, pos.y + 5);
        tft.setTextColor(WHITE);
        tft.print(lbl);
      }
    } else {
      tft.setCursor(pos.x + 5, pos.y + 5);
      tft.print(lbl);
    }
  }
}

// Erases aircraft from canvas
void Aircraft::erase() {
	// If location not set >> return
	if(!locationSet || pos.x == 0 || pos.y == 0)
		return;
	
	// If location set and outside drawing area >> return
	if(locationSet && posDistance >= TFT_DRAWABLE)
		return;

	// Erase aircraft
	tft.fillCircle(pos.x, pos.y, AIRCRAFT_SIZE, BLACK);	

	// If vector set and outside drawing area >> return
	if(vectorSet && vectDistance >= TFT_DRAWABLE)
		return;

	// Erase vector	
	if(vectorSet) tft.drawLine(pos.x, pos.y, vec.x, vec.y, BLACK);

	/* Erase Altitude Label */
	if(posDistance <= TFT_LBL_DRAWABLE) {		
		tft.setTextSize(1); tft.setTextColor(BLACK);
		if(vectorSet) {	

			if(vector.vrate >= 100) {				
				tft.fillCircle(pos.x + 12, pos.y, 2, BLACK);
				tft.setCursor(pos.x + 5, pos.y + 5);
				tft.print(lbl);
			}
			else if(vector.vrate <= -100) {				
				tft.fillCircle(pos.x + 12, pos.y, 2, BLACK);
				tft.setCursor(pos.x + 5, pos.y + 5);
				tft.print(lbl);
			}
			else {
				tft.setCursor(pos.x + 5, pos.y + 5);
				tft.print(lbl);
			}
		}
		else {			
			tft.setCursor(pos.x + 5, pos.y + 5);
			tft.print(lbl);
		}

	}
}

// Updates aircraft position on screen by removing and redrawing it
void Aircraft::update() {	
	if(locationSet) {
		erase();
		getXY();
	}

	if(vectorSet) getVectorXY();
	if(locationSet) draw();
}
#pragma endregion Functions




//#######################################################################
// Setter
//#######################################################################
#pragma region Setter
// Sets aircraft location
void Aircraft::setLocation(location_t newLocation) {
	
	lastSeen = seconds();	
	
	if(newLocation != location ) {
		
		if(locationSet) erase();
		else locationSet = true;

		location = newLocation;

		getXY();		
		if(vectorSet) getVectorXY();
		draw();
	}
}

// Sets aircraft vector
void Aircraft::setVector(vector_t newVector) {

	lastSeen = seconds();	
	
	if(newVector != vector ) {		
		
		if(locationSet) erase();
		vectorSet = true;

		vector = newVector;

		getVectorXY();
		
		if(locationSet) draw();
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

  /* Calculate cartesian X,Y from LAT & LON */
  xF = ((radians(location.lon) - radians(my.location.lon)) * cos((radians(my.location.lat) + radians(location.lat)) / 2)) * EARTH_RAD_NM;
  yF = (radians(location.lat) - radians(my.location.lat)) * EARTH_RAD_NM;
  dF = sqrt(xF * xF + yF * yF);

  /* Round and scale to selected range */
  pos.x = TFT_X_CENTER + round(xF * (tft.height() / 2) / states.currentRange);
  pos.y = TFT_Y_CENTER - round(yF * (tft.height() / 2) / states.currentRange);

  posDistance = round(dF * (TFT_Y_CENTER) / states.currentRange);
}

// Gets current aircraft vector
void Aircraft::getVectorXY() {

	uint8_t vectorLength = vector.speed / VECTOR_SIZE;
    
	float vectorX = ( cos( radians( vector.heading - 90 ) ) * vectorLength ) + pos.x;
	float vectorY = ( sin( radians( vector.heading - 90 ) ) * vectorLength ) + pos.y;

	vec.x = round( vectorX );
	vec.y = round( vectorY );

	float vdist = sqrt( pow(vec.x - TFT_X_CENTER, 2) + pow(vec.y - TFT_Y_CENTER, 2) );
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