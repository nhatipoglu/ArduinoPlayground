#include "DistanceAlert.h"

DistanceAlert::DistanceAlert(int trigPin, int echoPin, int buzzerPin)
    : trigPin_(trigPin),
      echoPin_(echoPin),
      buzzerPin_(buzzerPin),
  alertDistanceCm_(10.0),
  beepDurationMs_(60),
  lastBeepTimeMs_(0),
  buzzerOn_(false) {}

void DistanceAlert::begin() {
  pinMode(trigPin_, OUTPUT);
  pinMode(echoPin_, INPUT);
  pinMode(buzzerPin_, OUTPUT);
  digitalWrite(buzzerPin_, LOW);
}

DistanceReading DistanceAlert::update() {
  long pulseDuration = measurePulseDuration();

  if (pulseDuration == 0) {
    Serial.println("No measurement");
    silenceBuzzer();
    delay(30);
    return {false, 0.0f, false};
  }

  float distanceCm = pulseDuration * 0.0343 / 2;

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  if (distanceCm < alertDistanceCm_) {
    beepByDistance(distanceCm);
  } else {
    silenceBuzzer();
  }

  delay(30);

  return {true, distanceCm, distanceCm < alertDistanceCm_};
}

long DistanceAlert::measurePulseDuration() const {
  digitalWrite(trigPin_, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin_, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_, LOW);

  return pulseIn(echoPin_, HIGH, 30000);
}

void DistanceAlert::silenceBuzzer() {
  digitalWrite(buzzerPin_, LOW);
  buzzerOn_ = false;
}

void DistanceAlert::beepByDistance(float distanceCm) {
  unsigned long now = millis();
  unsigned long waitDurationMs = map((long)(distanceCm * 10), 0, 100, 40, 320);

  if (buzzerOn_) {
    if (now - lastBeepTimeMs_ >= beepDurationMs_) {
      digitalWrite(buzzerPin_, LOW);
      buzzerOn_ = false;
      lastBeepTimeMs_ = now;
    }
    return;
  }

  if (now - lastBeepTimeMs_ >= waitDurationMs) {
    digitalWrite(buzzerPin_, HIGH);
    buzzerOn_ = true;
    lastBeepTimeMs_ = now;
  }
}