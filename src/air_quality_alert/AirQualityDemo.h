#ifndef AIR_QUALITY_DEMO_H
#define AIR_QUALITY_DEMO_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "AirQualityAlert.h"
#include "../air_quality_leds/AirQualityLeds.h"

class AirQualityDemo {
 public:
  AirQualityDemo(int sensorPin,
                 int buzzerPin,
                 int whiteLedPin,
                 int redLedPin,
                 int lcdPrimaryAddress,
                 int lcdFallbackAddress);

  void begin();
  void update();

 private:
  int lcdPrimaryAddress_;
  int lcdFallbackAddress_;
  LiquidCrystal_I2C lcdPrimary_;
  LiquidCrystal_I2C lcdFallback_;
  LiquidCrystal_I2C* lcd_;
  bool lcdAvailable_;
  unsigned long lastLcdUpdateMs_;
  unsigned long lcdUpdateIntervalMs_;
  AirQualityAlert alert_;
  AirQualityLeds leds_;

  bool isI2cDeviceAvailable(int address);
  void blinkBacklightTest();
  void showStartupScreen(bool hasPrimary);
  void updateDisplay(const AirQualityReading& reading);
};

#endif