#ifndef DISTANCE_ALERT_H
#define DISTANCE_ALERT_H

#include <Arduino.h>

struct DistanceReading {
  bool hasMeasurement;
  float distanceCm;
  bool alertActive;
};

class DistanceAlert {
 public:
  DistanceAlert(int trigPin, int echoPin, int buzzerPin);

  void begin();
  DistanceReading update();

 private:
  int trigPin_;
  int echoPin_;
  int buzzerPin_;
  float alertDistanceCm_;
  unsigned long beepDurationMs_;
  unsigned long lastBeepTimeMs_;
  bool buzzerOn_;

  long measurePulseDuration() const;
  void silenceBuzzer();
  void beepByDistance(float distanceCm);
};

#endif