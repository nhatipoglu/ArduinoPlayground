#include "DistanceDemo.h"

DistanceDemo::DistanceDemo(int trigPin,
                           int echoPin,
                           int buzzerPin,
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
      alert_(trigPin, echoPin, buzzerPin) {}

void DistanceDemo::begin() {
  alert_.begin();

  bool hasPrimary = isI2cDeviceAvailable(lcdPrimaryAddress_);
  bool hasFallback = isI2cDeviceAvailable(lcdFallbackAddress_);

  Serial.println("Active demo: Distance");
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

void DistanceDemo::update() {
  DistanceReading reading = alert_.update();

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

bool DistanceDemo::isI2cDeviceAvailable(int address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

void DistanceDemo::blinkBacklightTest() {
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

void DistanceDemo::showStartupScreen(bool hasPrimary) {
  if (!lcdAvailable_) {
    return;
  }

  lcd_->clear();
  lcd_->setCursor(0, 0);
  lcd_->print("Demo hazir");
  lcd_->setCursor(0, 1);
  lcd_->print("Mesafe Alarmi ");
  delay(3000);

  lcd_->clear();
  lcd_->setCursor(0, 0);
  lcd_->print(hasPrimary ? "LCD 0x27 OK    " : "LCD 0x3F OK    ");
  lcd_->setCursor(0, 1);
  lcd_->print("Secim kaydedildi");
  delay(1500);
}

void DistanceDemo::updateDisplay(const DistanceReading& reading) {
  lcd_->setCursor(0, 0);
  if (!reading.hasMeasurement) {
    lcd_->print("Mesafe yok    ");
  } else {
    lcd_->print("Mesafe:       ");
    lcd_->setCursor(8, 0);
    lcd_->print(reading.distanceCm, 1);
    lcd_->print("cm ");
  }

  lcd_->setCursor(0, 1);
  if (!reading.hasMeasurement) {
    lcd_->print("HC-SR04 bekliyor");
  } else if (reading.alertActive) {
    lcd_->print("Yakin: Bip aktif");
  } else {
    lcd_->print("Durum: Guvenli ");
  }
}