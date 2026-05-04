# Distance Alert

Bu ozellik HC-SR04 ultrasonik sensor ile mesafe olcup nesne yaklastikca buzzer uyarisi verir. Su anda ana sketch tarafinda aktif degil ama alternatif demo modulu olarak projede tutuluyor.

## Kullanilan Dosyalar

- `DistanceAlert.h`: veri modeli ve sinif arayuzu
- `DistanceAlert.cpp`: olcum, timeout ve bip araligi mantigi
- `DistanceDemo.h`: mesafe demosunun ust seviye sinifi
- `DistanceDemo.cpp`: LCD ve sensor akisini birlestiren uygulama mantigi

## Donanim

- `TRIG` -> secilen dijital pin
- `ECHO` -> secilen dijital pin
- Buzzer -> secilen dijital pin

## Davranis

- `alertDistanceCm_ = 10.0` altinda buzzer aktif olur.
- Mesafe azaldikca bekleme suresi kisalir.
- Okuma yoksa seri hatta `No measurement` yazilir.
- LCD varsa mesafe ve durum 16x2 ekranda gosterilir.

## Mimari

- `DistanceAlert`: sensor ve buzzer mantigini tutar.
- `DistanceDemo`: `DistanceAlert` ve LCD akislarini bir araya getirir.
- Ana sketch sadece pinleri verip `begin()` ve `update()` cagirir.

## Not

Bu modul artik ana sketch icinde hazir durumda. Aktif etmek icin [ArduinoPlayground.ino](ArduinoPlayground.ino) icindeki secimi soyle degistirmen yeterlidir:

```cpp
const DemoMode activeDemo = DEMO_DISTANCE;
```