#ifndef MicroUtils_h
#define MicroUtils_h

#include "LEDRing.h"
#include <Arduino.h>
#include <FastLED.h>

typedef LEDRing *LEDRingPtr;

static CHSV fromHSV(int hue, int sat, int val) {
  byte h = map(hue, 0, 360, 0, 255);
  byte s = map(sat, 0, 100, 0, 255);
  byte v = map(val, 0, 100, 0, 255);

  return CHSV(h, s, v);
}
#endif
