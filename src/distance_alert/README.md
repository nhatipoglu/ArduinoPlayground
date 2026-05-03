# Distance Alert

Bu ozellik HC-SR04 ultrasonik sensor ile mesafe olcup nesne yaklastikca buzzer uyarisi verir. Su anda ana sketch tarafinda aktif degil ama alternatif demo modulu olarak projede tutuluyor.

## Kullanilan Dosyalar

- `DistanceAlert.h`: veri modeli ve sinif arayuzu
- `DistanceAlert.cpp`: olcum, timeout ve bip araligi mantigi

## Donanim

- `TRIG` -> secilen dijital pin
- `ECHO` -> secilen dijital pin
- Buzzer -> secilen dijital pin

## Davranis

- `alertDistanceCm_ = 10.0` altinda buzzer aktif olur.
- Mesafe azaldikca bekleme suresi kisalir.
- Okuma yoksa seri hatta `No measurement` yazilir.

## Not

Bu modulun tekrar aktif edilmesi icin ana sketch icinde uygun include, nesne olusturma ve `loop()` entegrasyonu yapilmasi yeterlidir.