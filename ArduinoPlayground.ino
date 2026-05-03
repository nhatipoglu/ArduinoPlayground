#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "AirQualityAlert.h"

const int airQualitySensorPin = A0;
const int buzzerPin = 9;

const int lcdPrimaryAddress = 0x27;
const int lcdFallbackAddress = 0x3F;

LiquidCrystal_I2C lcdPrimary(lcdPrimaryAddress, 16, 2);
LiquidCrystal_I2C lcdFallback(lcdFallbackAddress, 16, 2);
LiquidCrystal_I2C* lcd = &lcdPrimary;
bool lcdAvailable = false;
unsigned long lastLcdUpdateMs = 0;

const unsigned long lcdUpdateIntervalMs = 300;

AirQualityAlert airQualityAlert(airQualitySensorPin, buzzerPin);

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
  lcd->print("LCD START OK");
  lcd->setCursor(0, 1);
  lcd->print(hasPrimary ? "ADDR 0x27" : "ADDR 0x3F");
  delay(3000);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  airQualityAlert.begin();

  bool hasPrimary = isI2cDeviceAvailable(lcdPrimaryAddress);
  bool hasFallback = isI2cDeviceAvailable(lcdFallbackAddress);

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
}