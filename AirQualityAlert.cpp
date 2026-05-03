#include "AirQualityAlert.h"

AirQualityAlert::AirQualityAlert(int sensorPin, int buzzerPin)
    : sensorPin_(sensorPin),
      buzzerPin_(buzzerPin),
  warningThresholdPercent_(45),
  dangerThresholdPercent_(75),
      beepDurationMs_(90),
      lastBeepTimeMs_(0),
      buzzerOn_(false) {}

void AirQualityAlert::begin() {
  pinMode(sensorPin_, INPUT);
  pinMode(buzzerPin_, OUTPUT);
  noTone(buzzerPin_);
}

AirQualityReading AirQualityAlert::update() {
  int rawValue = readSensor();
  int qualityPercent = calculateQualityPercent(rawValue);
  bool alertActive = qualityPercent >= warningThresholdPercent_;
  bool dangerActive = qualityPercent >= dangerThresholdPercent_;
  const char* statusLabel = "TEMIZ";

  if (dangerActive) {
    statusLabel = "TEHLIKE";
  } else if (alertActive) {
    statusLabel = "UYARI";
  }

  Serial.print("Air raw: ");
  Serial.print(rawValue);
  Serial.print(" quality: ");
  Serial.print(qualityPercent);
  Serial.print("% status: ");
  Serial.println(statusLabel);

  if (alertActive) {
    updateBuzzer(qualityPercent);
  } else {
    silenceBuzzer();
  }

  delay(150);

  return {true, rawValue, qualityPercent, alertActive, dangerActive};
}

int AirQualityAlert::readSensor() const {
  return analogRead(sensorPin_);
}

int AirQualityAlert::calculateQualityPercent(int rawValue) const {
  return map(constrain(rawValue, 0, 1023), 0, 1023, 0, 100);
}

void AirQualityAlert::silenceBuzzer() {
  noTone(buzzerPin_);
  buzzerOn_ = false;
}

void AirQualityAlert::updateBuzzer(int qualityPercent) {
  unsigned long now = millis();
  unsigned long waitDurationMs = map(
      constrain(qualityPercent, warningThresholdPercent_, 100),
      warningThresholdPercent_, 100, 450, 120);

  if (qualityPercent >= dangerThresholdPercent_) {
    tone(buzzerPin_, 2200);
    buzzerOn_ = true;
    lastBeepTimeMs_ = now;
    return;
  }

  if (buzzerOn_) {
    if (now - lastBeepTimeMs_ >= beepDurationMs_) {
      noTone(buzzerPin_);
      buzzerOn_ = false;
      lastBeepTimeMs_ = now;
    }
    return;
  }

  if (now - lastBeepTimeMs_ >= waitDurationMs) {
    tone(buzzerPin_, 1800);
    buzzerOn_ = true;
    lastBeepTimeMs_ = now;
  }
}