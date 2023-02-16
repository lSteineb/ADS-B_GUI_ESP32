#ifndef _TOUCH_
#define _TOUCH_

#define TS_MINX 3850
#define TS_MAXX 120
#define TS_MINY 170
#define TS_MAXY 3900

#define TFT_WIDTH2 480
#define TFT_HEIGHT2 320

// ISR function pointer
typedef void (*isr_t)();


class ADSP_STMPE610 : public Adafruit_STMPE610 {
private:
  int INT_PIN;
  isr_t ISR;
  volatile bool ISRflag = false;

  uint16_t width, height;

public:
  ADESP_STMPE610(int CS)
    : Adafruit_STMPE610(CS) {}
  ADESP_STMPE610(int CS, int STMPE_INT, isr_t ISR);

  void clearBuffer(void);
  void interruptReset(void);
  void enableISR(uint8_t intPin, isr_t ISR);

  bool ISR_Flag(void);
  void setISRflag(bool);

  TS_Point getTSPoint();
};

#endif