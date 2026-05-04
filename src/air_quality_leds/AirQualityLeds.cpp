#include "AirQualityLeds.h"

AirQualityLeds::AirQualityLeds(int whiteLedPin, int redLedPin)
    : whiteLedPin_(whiteLedPin), redLedPin_(redLedPin) {}

void AirQualityLeds::begin() {
  pinMode(whiteLedPin_, OUTPUT);
  digitalWrite(whiteLedPin_, LOW);

  pinMode(redLedPin_, OUTPUT);
  digitalWrite(redLedPin_, LOW);
}

void AirQualityLeds::update(const AirQualityReading& reading) {
  if (!reading.hasMeasurement) {
    digitalWrite(whiteLedPin_, LOW);
    digitalWrite(redLedPin_, LOW);
    return;
  }

  if (!reading.alertActive) {
    digitalWrite(whiteLedPin_, HIGH);
    digitalWrite(redLedPin_, LOW);
    return;
  }

  digitalWrite(whiteLedPin_, LOW);
  digitalWrite(redLedPin_, HIGH);
}