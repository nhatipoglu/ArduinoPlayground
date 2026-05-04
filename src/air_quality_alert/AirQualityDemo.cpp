#include "AirQualityDemo.h"

AirQualityDemo::AirQualityDemo(int sensorPin,
                               int buzzerPin,
                               int whiteLedPin,
                               int redLedPin,
                               int lcdPrimaryAddress,
                               int lcdFallbackAddress)
    : lcdPrimaryAddress_(lcdPrimaryAddress),
      lcdFallbackAddress_(lcdFallbackAddress),
      lcdPrimary_(lcdPrimaryAddress, 16, 2),
      lcdFallback_(lcdFallbackAddress, 16, 2),
      lcd_(&lcdPrimary_),
      lcdAvailable_(false),
      lastLcdUpdateMs_(0),
      lcdUpdateIntervalMs_(300),
      alert_(sensorPin, buzzerPin),
      leds_(whiteLedPin, redLedPin) {}

void AirQualityDemo::begin() {
  alert_.begin();
  leds_.begin();

  bool hasPrimary = isI2cDeviceAvailable(lcdPrimaryAddress_);
  bool hasFallback = isI2cDeviceAvailable(lcdFallbackAddress_);

  Serial.println("Active demo: Air Quality");
  Serial.println("LCD check start");
  Serial.print("0x27: ");
  Serial.println(hasPrimary ? "OK" : "not found");
  Serial.print("0x3F: ");
  Serial.println(hasFallback ? "OK" : "not found");

  if (hasFallback && !hasPrimary) {
    lcd_ = &lcdFallback_;
  }

  lcdAvailable_ = hasPrimary || hasFallback;

  if (!lcdAvailable_) {
    Serial.println("No LCD found on I2C bus");
    return;
  }

  lcd_->init();
  lcd_->backlight();
  blinkBacklightTest();
  showStartupScreen(hasPrimary);
}

void AirQualityDemo::update() {
  AirQualityReading reading = alert_.update();
  leds_.update(reading);

  if (!lcdAvailable_) {
    return;
  }

  unsigned long now = millis();
  if (now - lastLcdUpdateMs_ < lcdUpdateIntervalMs_) {
    return;
  }

  lastLcdUpdateMs_ = now;
  updateDisplay(reading);
}

bool AirQualityDemo::isI2cDeviceAvailable(int address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

void AirQualityDemo::blinkBacklightTest() {
  if (!lcdAvailable_) {
    return;
  }

  for (int index = 0; index < 3; ++index) {
    lcd_->noBacklight();
    delay(250);
    lcd_->backlight();
    delay(250);
  }
}

void AirQualityDemo::showStartupScreen(bool hasPrimary) {
  if (!lcdAvailable_) {
    return;
  }

  lcd_->clear();
  lcd_->setCursor(0, 0);
  lcd_->print("Demo hazir");
  lcd_->setCursor(0, 1);
  lcd_->print("Hava Kalitesi ");
  delay(3000);

  lcd_->clear();
  lcd_->setCursor(0, 0);
  lcd_->print(hasPrimary ? "LCD 0x27 OK    " : "LCD 0x3F OK    ");
  lcd_->setCursor(0, 1);
  lcd_->print("Secim kaydedildi");
  delay(1500);
}

void AirQualityDemo::updateDisplay(const AirQualityReading& reading) {
  lcd_->setCursor(0, 0);
  if (reading.hasMeasurement) {
    lcd_->print("Hava:         ");
    lcd_->setCursor(6, 0);
    lcd_->print(reading.qualityPercent);
    lcd_->print("%   ");
  } else {
    lcd_->print("Sensor yok    ");
  }

  lcd_->setCursor(0, 1);
  if (!reading.hasMeasurement) {
    lcd_->print("Sensor bekliyor");
  } else if (reading.dangerActive) {
    lcd_->print("Tehlike: Kirli ");
  } else if (reading.alertActive) {
    lcd_->print("Uyari: Havalandir");
  } else {
    lcd_->print("Durum: Temiz   ");
  }
}