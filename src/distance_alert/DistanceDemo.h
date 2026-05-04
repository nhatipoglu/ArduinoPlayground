#ifndef DISTANCE_DEMO_H
#define DISTANCE_DEMO_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "DistanceAlert.h"

class DistanceDemo {
 public:
  DistanceDemo(int trigPin,
               int echoPin,
               int buzzerPin,
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
  DistanceAlert alert_;

  bool isI2cDeviceAvailable(int address);
  void blinkBacklightTest();
  void showStartupScreen(bool hasPrimary);
  void updateDisplay(const DistanceReading& reading);
};

#endif