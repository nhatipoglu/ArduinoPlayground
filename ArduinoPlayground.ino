#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "src/air_quality_alert/AirQualityDemo.h"
#include "src/distance_alert/DistanceDemo.h"

const int airQualitySensorPin = A0;
const int airQualityBuzzerPin = 9;
const int airQualityWhiteLedPin = 4;
const int airQualityRedLedPin = 5;

const int distanceTrigPin = 11;
const int distanceEchoPin = 12;
const int distanceBuzzerPin = 8;

enum DemoMode { DEMO_AIR_QUALITY, DEMO_DISTANCE };

const DemoMode activeDemo = DEMO_AIR_QUALITY;

const int lcdPrimaryAddress = 0x27;
const int lcdFallbackAddress = 0x3F;

AirQualityDemo airQualityDemo(airQualitySensorPin,
                              airQualityBuzzerPin,
                              airQualityWhiteLedPin,
                              airQualityRedLedPin,
                              lcdPrimaryAddress,
                              lcdFallbackAddress);
DistanceDemo distanceDemo(distanceTrigPin,
                          distanceEchoPin,
                          distanceBuzzerPin,
                          lcdPrimaryAddress,
                          lcdFallbackAddress);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (activeDemo == DEMO_AIR_QUALITY) {
    airQualityDemo.begin();
  } else {
    distanceDemo.begin();
  }
}

void loop() {
  if (activeDemo == DEMO_AIR_QUALITY) {
    airQualityDemo.update();
    return;
  }

  distanceDemo.update();
}