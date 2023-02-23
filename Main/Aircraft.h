#ifndef _AIRCRAFT_
#define _AIRCRAFT_

#include <cmath>
#include <vector>

#include "UI_Types.h"

class Aircraft {
public:
  Aircraft();
  Aircraft(vector_t);
  Aircraft(location_t);

  void draw(void);
  void erase(void);
  void update(void);
  bool checkCollision(point_t);

  void setVector(vector_t);
  void setLocation(location_t);
  void setInformation(information_t);
  void setSelected(bool);
  
  uint16_t getLastSeen(void);
  vector_t getVector(void);
  location_t getLocation(void);
  information_t getInformation(void);
  bool getSelected(void);
  void displayInformation(void);
  void drawTrail(uint32_t);


private:
  int16_t lbl;
  vector_t vector;
  location_t location;
  information_t information;
  std::vector<point_t> previousPos;

  bool locationSet = false;
  bool vectorSet = false;
  bool informationSet = false;
  bool selected = false;
  bool emergency = false;

  point_t pos, vec;
  uint16_t posDistance = 0;
  uint16_t vectDistance = 0;
  uint16_t lastSeen = 0;

  void getVectorXY();
  void getXY();
};

#endif