# Air Quality Alert

Bu ozellik analog hava kalitesi sensorunden okuma alip sonucu yuzdeye cevirir. Esik degerler asildiginda buzzer ile uyari verir ve ana sketch uzerinden LCD ile seri monitore durum aktarir.

LED yonetimi bu moduldedir degil; ayri olarak `src/air_quality_leds/` altinda tutulur.

## Kullanilan Dosyalar

- `AirQualityAlert.h`: veri modeli ve sinif arayuzu
- `AirQualityAlert.cpp`: sensor okuma, yuzde hesaplama ve buzzer mantigi
- `AirQualityDemo.h`: hava kalitesi demosunun ust seviye sinifi
- `AirQualityDemo.cpp`: LCD, LED ve sensor akisini birlestiren uygulama mantigi

## Donanim

- Analog hava kalitesi sensoru cikisi -> `A0`
- Buzzer -> `D9`
- Beyaz LED -> `D4`
- Kirmizi LED -> `D5`
- GND ve 5V baglantilari

## Davranis

- Temiz havada sadece beyaz LED yanar.
- `warningThresholdPercent_` asildiginda beyaz LED kapanir ve kirmizi LED yanar.
- `warningThresholdPercent_ = 45` ustunde aralikli uyari baslar.
- `dangerThresholdPercent_ = 75` ustunde surekli tona gecilir.
- Seri monitor ciktisi `Air raw: ... quality: ... status: ...` formatindadir.

## Mimari

- `AirQualityAlert`: sensor ve buzzer mantigini tutar.
- `AirQualityDemo`: `AirQualityAlert` ve LED/LCD akislarini bir araya getirir.
- Ana sketch sadece pinleri verip `begin()` ve `update()` cagirir.

## Nereden Aktif Ediliyor

Ana senaryo [ArduinoPlayground.ino](ArduinoPlayground.ino) icinde `AirQualityDemo airQualityDemo(...)` ile olusturulur ve `loop()` icinde `update()` cagrilir.

Bu demoyu secmek icin ayni dosyada su satir aktif olmali:

```cpp
const DemoMode activeDemo = DEMO_AIR_QUALITY;
```