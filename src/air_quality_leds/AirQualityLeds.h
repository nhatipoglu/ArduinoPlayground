#ifndef AIR_QUALITY_LEDS_H
#define AIR_QUALITY_LEDS_H

#include <Arduino.h>

#include "../air_quality_alert/AirQualityAlert.h"

class AirQualityLeds {
 public:
  AirQualityLeds(int whiteLedPin, int redLedPin);

  void begin();
  void update(const AirQualityReading& reading);

 private:
  int whiteLedPin_;
  int redLedPin_;
};

#endif