#include <string>
#include "stdint.h"
#ifndef _UI_TYPES_
#define _UI_TYPES_

#include "ILI9488.h"

// LCD 16 BIT COLORS
#define BLACK 0x0000
#define WHITE 0xFFFF
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define GREEN2 0x0523
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define YELLOW2 0xFEA0
#define YELLOW3 0xCEA0
#define YELLOW4 0xFEE0
#define GREY 0x8410
#define DGREY 0x528A
#define DGREY2 0x2104
#define SKYBLUE 0x041F
#define GNDBROWN 0x8208

// RGB to 565 coversion
static inline uint16_t RGB(uint8_t R, uint8_t G, uint8_t B) {
  return ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3);
}

//#####################################################################
// UI COLOR DEFINITIONS
//#####################################################################
#define UI_BACKGROUND_COLOR RGB(60, 60, 60)
#define UI_TOP_BAR_COLOR RGB(0, 0, 0)
#define UI_TOP_BAR_DIV_LINE_COLOR RGB(150, 150, 150)
#define UI_OUTER_CIRCLE_COLOR RGB(150, 150, 150)
#define UI_INNER_CIRCLE_COLOR RGB(255, 255, 255)
#define UI_MY_AIRCRAFT_COLOR RGB(255, 0, 0)

#define UI_BUTTON_FILL_COLOR RGB(70, 70, 70)

// UI DEFINES
#define AIRCRAFT_SIZE 6
#define VECTOR_SIZE 20


#define MIN_RANGE 10  // aprox. 19 km
#define MAX_RANGE 140 // aprox. 260 km
#define DEFAULT_RANGE 50 // aprox 92 km

// EARTH RADIUS
// Metric (m, km, km/h)
// Imperial (ft, NM, knots)
#define EARTH_RAD_KM 6371
#define EARTH_RAD_NM 3440


//#####################################################################
// Structs
//#####################################################################

struct screenRegion_t {
  int xL, xR;
  int yT, yB;
};

struct point_t {
  uint16_t x, y;
};

struct location_t {
  int alt;
  float lat, lon;
};

struct vector_t {
  uint16_t speed, heading;
  int16_t vrate;
};

struct information_t {
  const char* icao_hex;
  uint16_t squawk;
  const char* flight;
  const char* category;
};

struct mydata_t {
  vector_t vector{ 0, 0, 0 };
  location_t location{ 0, 0.00, 0.00 };
};

struct programdata_t {
  uint8_t currentRange = DEFAULT_RANGE;
  // TODO: add more data
};

//#####################################################################
// Operators
//#####################################################################

inline bool operator==(const point_t &ts, const screenRegion_t &reg) {
  return ts.x > reg.xL && ts.x < reg.xR && ts.y > reg.yT && ts.y < reg.yB;
};

static inline bool operator!=(const location_t &loc1, const location_t &loc2) {
  return loc1.lat != loc2.lat || loc1.lon != loc2.lon;
};

static inline bool operator!=(const vector_t &vect1, const vector_t &vect2) {
  return vect1.speed != vect2.speed || vect1.heading != vect2.heading || vect1.vrate != vect2.vrate;
};

static inline bool operator!=(const information_t &info1, const information_t &info2) {
  return info1.icao_hex != info2.icao_hex || info1.squawk != info2.squawk || info1.flight != info2.flight || info1.category != info2.category;
};

#endif