#ifndef GUI
#define GUI
#include <cmath>

#define EARTH_RAD_KM	6371
#define EARTH_RAD_NM	3440

struct location_t {
	int alt;
	float lat, lon;
};

location_t myloc{0, 51.191847, 6.808780};

void drawBaseUI(Display tft){
  tft.fillScreen(TFT_BLUE);	
		
	// Top Black Bar
	tft.fillRect(0,0, TFT_HEIGHT, 30, TFT_DARKGREY);	
	tft.drawFastHLine(0, 30, TFT_HEIGHT, TFT_WHITE);

	// Top bar text
	tft.setTextSize(2); tft.setTextColor(TFT_WHITE);
	// System Name
	tft.setCursor(10, 10); tft.print("TRAFFIC");

	// Restore Default Font
	tft.setTextSize(1);
	tft.setTextColor(TFT_WHITE);

  // Draw radarcircle
  tft.drawCircle(tft.width()/2, tft.height()/2, tft.height()/2-1, TFT_BLACK);
  tft.fillCircle(tft.width()/2, tft.height()/2, tft.height()/2-1, TFT_BLACK);
  // Draw current position
	tft.drawCircle(tft.width()/2, tft.height()/2, 3, TFT_RED);

  // Inner Circle Params
	int degMark = 15;
	int ri = TFT_WIDTH / 4;
	// Draw inner circle
	for (int i = 0; i<360; i += degMark) {			
		float a = radians(i);
		tft.drawPixel(TFT_X_CENTER + cos(a)*ri, TFT_Y_CENTER + sin(a)*ri, TFT_WHITE);
	}

  // Draw range text with nautical miles as units
  int offset = 12;	
  tft.setTextSize(2); tft.setTextColor(WHITE);	
  tft.setCursor(tft.width() - tft.width()/3 + 18 + offset, tft.height() - tft.height()/10);
  tft.print("50");		
  tft.setCursor(tft.width() - tft.width()/3 + 58, tft.height() - tft.height()/10);
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
std::pair<float, float>  getXY(float lat, float lon) {	
  std::pair<float, float> result;
	float xF, yF, dF;
	
	xF = ((radians(lon) - radians(myloc.lon)) * cos((radians(myloc.lat) + radians(lat)) / 2)) * EARTH_RAD_NM;
	yF = (radians(lat) - radians(myloc.lat)) * EARTH_RAD_NM;
	dF = sqrt( xF*xF + yF*yF );

	/* Round and scale to selected range */
	result.first = TFT_X_CENTER + round(xF * TFT_Y_CENTER / 50);
	result.second = TFT_Y_CENTER - round(yF * TFT_Y_CENTER / 50);
	
	float posDistance = round(dF * TFT_Y_CENTER / 50);

  return result;
}


#endif