#ifndef AIR_QUALITY_ALERT_H
#define AIR_QUALITY_ALERT_H

#include <Arduino.h>

struct AirQualityReading {
  bool hasMeasurement;
  int rawValue;
  int qualityPercent;
  bool alertActive;
  bool dangerActive;
};

class AirQualityAlert {
 public:
  AirQualityAlert(int sensorPin, int buzzerPin);

  void begin();
  AirQualityReading update();

 private:
  int sensorPin_;
  int buzzerPin_;
  int warningThresholdPercent_;
  int dangerThresholdPercent_;
  unsigned long beepDurationMs_;
  unsigned long lastBeepTimeMs_;
  bool buzzerOn_;

  int readSensor() const;
  int calculateQualityPercent(int rawValue) const;
  void silenceBuzzer();
  void updateBuzzer(int qualityPercent);
};

#endif