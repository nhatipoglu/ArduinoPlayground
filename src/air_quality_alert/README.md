# Air Quality Alert

Bu ozellik analog hava kalitesi sensorunden okuma alip sonucu yuzdeye cevirir. Esik degerler asildiginda buzzer ile uyari verir ve ana sketch uzerinden LCD ile seri monitore durum aktarir.

## Kullanilan Dosyalar

- `AirQualityAlert.h`: veri modeli ve sinif arayuzu
- `AirQualityAlert.cpp`: sensor okuma, yuzde hesaplama ve buzzer mantigi

## Donanim

- Analog hava kalitesi sensoru cikisi -> `A0`
- Buzzer -> `D9`
- GND ve 5V baglantilari

## Davranis

- `warningThresholdPercent_ = 45` ustunde aralikli uyari baslar.
- `dangerThresholdPercent_ = 75` ustunde surekli tona gecilir.
- Seri monitor ciktisi `Air raw: ... quality: ... status: ...` formatindadir.

## Nereden Aktif Ediliyor

Ana senaryo [ArduinoPlayground.ino](ArduinoPlayground.ino) icinde `AirQualityAlert airQualityAlert(...)` ile olusturulur ve `loop()` icinde `update()` cagrilir.

Bu demoyu secmek icin ayni dosyada su satir aktif olmali:

```cpp
const DemoMode activeDemo = DEMO_AIR_QUALITY;
```