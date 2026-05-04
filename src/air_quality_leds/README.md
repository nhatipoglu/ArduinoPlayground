# Air Quality Leds

Bu modul hava kalitesi durumuna gore iki LED'i yonetir.

## Kullanilan Dosyalar

- `AirQualityLeds.h`: LED sinifi arayuzu
- `AirQualityLeds.cpp`: LED pin baslatma ve durum guncelleme mantigi

## Donanim

- Beyaz LED -> `D4`
- Kirmizi LED -> `D5`

## Davranis

- Sensor verisi yoksa iki LED de kapanir.
- Hava temizse sadece beyaz LED yanar.
- `alertActive` oldugunda beyaz LED kapanir ve kirmizi LED yanar.