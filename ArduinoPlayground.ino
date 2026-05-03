#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "src/air_quality_alert/AirQualityAlert.h"
#include "src/distance_alert/DistanceAlert.h"

const int airQualitySensorPin = A0;
const int airQualityBuzzerPin = 9;

const int distanceTrigPin = 11;
const int distanceEchoPin = 12;
const int distanceBuzzerPin = 8;

enum DemoMode {
  DEMO_AIR_QUALITY,
  DEMO_DISTANCE,
};

const DemoMode activeDemo = DEMO_AIR_QUALITY;

const int lcdPrimaryAddress = 0x27;
const int lcdFallbackAddress = 0x3F;

LiquidCrystal_I2C lcdPrimary(lcdPrimaryAddress, 16, 2);
LiquidCrystal_I2C lcdFallback(lcdFallbackAddress, 16, 2);
LiquidCrystal_I2C* lcd = &lcdPrimary;
bool lcdAvailable = false;
unsigned long lastLcdUpdateMs = 0;

const unsigned long lcdUpdateIntervalMs = 300;

AirQualityAlert airQualityAlert(airQualitySensorPin, airQualityBuzzerPin);
DistanceAlert distanceAlert(distanceTrigPin, distanceEchoPin, distanceBuzzerPin);

const char* getDemoLabel() {
  return activeDemo == DEMO_AIR_QUALITY ? "Air Quality" : "Distance";
}

bool isI2cDeviceAvailable(int address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

void blinkBacklightTest() {
  if (!lcdAvailable) {
    return;
  }

  for (int i = 0; i < 3; ++i) {
    lcd->noBacklight();
    delay(250);
    lcd->backlight();
    delay(250);
  }
}

void showStartupScreen(bool hasPrimary) {
  if (!lcdAvailable) {
    return;
  }

  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Demo hazir");
  lcd->setCursor(0, 1);
  lcd->print(activeDemo == DEMO_AIR_QUALITY ? "Hava Kalitesi " : "Mesafe Alarmi ");
  delay(3000);

  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(hasPrimary ? "LCD 0x27 OK    " : "LCD 0x3F OK    ");
  lcd->setCursor(0, 1);
  lcd->print("Secim kaydedildi");
  delay(1500);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (activeDemo == DEMO_AIR_QUALITY) {
    airQualityAlert.begin();
  } else {
    distanceAlert.begin();
  }

  bool hasPrimary = isI2cDeviceAvailable(lcdPrimaryAddress);
  bool hasFallback = isI2cDeviceAvailable(lcdFallbackAddress);

  Serial.print("Active demo: ");
  Serial.println(getDemoLabel());
  Serial.println("LCD check start");
  Serial.print("0x27: ");
  Serial.println(hasPrimary ? "OK" : "not found");
  Serial.print("0x3F: ");
  Serial.println(hasFallback ? "OK" : "not found");

  if (hasFallback && !hasPrimary) {
    lcd = &lcdFallback;
  }

  lcdAvailable = hasPrimary || hasFallback;

  if (!lcdAvailable) {
    Serial.println("No LCD found on I2C bus");
    return;
  }

  lcd->init();
  lcd->backlight();
  blinkBacklightTest();
  showStartupScreen(hasPrimary);
}

void loop() {
  if (activeDemo == DEMO_AIR_QUALITY) {
    AirQualityReading reading = airQualityAlert.update();

    if (!lcdAvailable) {
      return;
    }

    unsigned long now = millis();
    if (now - lastLcdUpdateMs < lcdUpdateIntervalMs) {
      return;
    }

    lastLcdUpdateMs = now;

    lcd->setCursor(0, 0);
    if (reading.hasMeasurement) {
      lcd->print("Hava:         ");
      lcd->setCursor(6, 0);
      lcd->print(reading.qualityPercent);
      lcd->print("%   ");
    } else {
      lcd->print("Sensor yok    ");
    }

    lcd->setCursor(0, 1);
    if (!reading.hasMeasurement) {
      lcd->print("Sensor bekliyor");
    } else if (reading.dangerActive) {
      lcd->print("Tehlike: Kirli ");
    } else if (reading.alertActive) {
      lcd->print("Uyari: Havaland");
    } else {
      lcd->print("Durum: Temiz   ");
    }

    return;
  }

  DistanceReading reading = distanceAlert.update();

  if (!lcdAvailable) {
    return;
  }

  unsigned long now = millis();
  if (now - lastLcdUpdateMs < lcdUpdateIntervalMs) {
    return;
  }

  lastLcdUpdateMs = now;

  lcd->setCursor(0, 0);
  if (!reading.hasMeasurement) {
    lcd->print("Mesafe yok    ");
  } else {
    lcd->print("Mesafe:       ");
    lcd->setCursor(8, 0);
    lcd->print(reading.distanceCm, 1);
    lcd->print("cm ");
  }

  lcd->setCursor(0, 1);
  if (!reading.hasMeasurement) {
    lcd->print("HC-SR04 bekliyor");
  } else if (reading.alertActive) {
    lcd->print("Yakin: Bip aktif");
  } else {
    lcd->print("Durum: Guvenli ");
  }
}