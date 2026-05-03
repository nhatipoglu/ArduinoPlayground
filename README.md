# ArduinoPlayground

Bu repo bir Arduino demo alani. Ana sketch su anda hava kalitesi uyarisi senaryosunu calistiriyor, ama alternatif ozellikler de ayri klasorlerde tutuluyor. Amac, her ozelligin kendi kodu ve kendi kucuk README'si ile bulunabilir olmasi.

## Proje Yapisi

```text
ArduinoPlayground/
|- ArduinoPlayground.ino
|- README.md
|- src/
|  |- air_quality_alert/
|  |  |- AirQualityAlert.h
|  |  |- AirQualityAlert.cpp
|  |  |- README.md
|  \- distance_alert/
|     |- DistanceAlert.h
|     |- DistanceAlert.cpp
|     \- README.md
|- build/
\- .vscode/
```

## Nereden Baslanir

- Ana giris noktasi `ArduinoPlayground.ino` dosyasidir.
- Aktif ozellik bu dosyada include edilmis olan moduldur.
- Her ozelligin detaylari kendi klasorundeki README icindedir.

Bu projede su anda:

- aktif demo: `src/air_quality_alert/`
- alternatif modul: `src/distance_alert/`

## Ozellikler

### Air Quality Alert

Bu mod ul analog hava kalitesi sensorunden veri alip sonucu yuzdeye cevirir, gerekirse buzzer ile uyari verir ve LCD ile seri monitore durum basar.

Detaylar icin: `src/air_quality_alert/README.md`

Kisa teknik ozet:

- sensor pini: `A0`
- buzzer pini: `D9`
- LCD I2C adres denemeleri: `0x27` ve `0x3F`
- seri hiz: `9600`

### Distance Alert

Bu mod ul HC-SR04 ile mesafe olcer ve nesne yaklastikca buzzeri daha sik caldirir. Su an aktif sketch icinde bagli degil ama alternatif demo mantigi olarak tutuluyor.

Detaylar icin: `src/distance_alert/README.md`

## Gerekli Olanlar

Projeyi terminalden derlemek ve karta yuklemek icin bunlar lazim:

- `arduino-cli`
- Arduino AVR core
- Arduino Uno veya Uno ile uyumlu bir kart
- USB kablosu
- Air Quality Alert icin analog hava kalitesi sensoru, buzzer, I2C LCD

Yuklu core'lari kontrol etmek icin:

```bash
arduino-cli core list
```

Eger AVR core yoksa tipik kurulum:

```bash
arduino-cli core install arduino:avr
```

## Karti Nasil Bulursun

Karti USB ile bagladiktan sonra terminalde:

```bash
arduino-cli board list
```

Bu komut portu gosterir. Linux tarafinda genelde su tipte gorunur:

- `/dev/ttyUSB0`
- `/dev/ttyACM0`

Klon kartlarda kart adi bazen tam taninmasa da port dogruysa upload yine calisabilir.

## Build Nasil Alinir

Repo kokunde terminal acip su komutu calistir:

```bash
arduino-cli compile --fqbn arduino:avr:uno --build-path "$PWD/build" "$PWD"
```

Basarili derlemede boyle bir ozet gorursun:

```text
Sketch uses ... bytes of program storage space.
Global variables use ... bytes of dynamic memory.
```

Derleme ciktilari `build/` klasorune yazilir. En kritik dosyalar:

- `build/ArduinoPlayground.ino.hex`
- `build/ArduinoPlayground.ino.elf`
- `build/ArduinoPlayground.ino.with_bootloader.hex`

## Koda Karta Nasil Atarsin

Once portu bul, sonra upload komutunu ver:

```bash
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno --input-dir "$PWD/build" "$PWD"
```

Portun farkliysa `/dev/ttyUSB0` yerine kendi portunu yaz.

Tipik akis:

1. `arduino-cli board list`
2. `arduino-cli compile --fqbn arduino:avr:uno --build-path "$PWD/build" "$PWD"`
3. `arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno --input-dir "$PWD/build" "$PWD"`

## Terminalden Ciktiya Nasil Bakilir

Kod seri porta `9600` baud ile log basiyor. Terminalden izlemek icin:

```bash
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=9600
```

Air Quality Alert aktifken ornek ciktilar su formda olur:

```text
LCD check start
0x27: OK
0x3F: not found
Air raw: 412 quality: 40% status: TEMIZ
Air raw: 690 quality: 67% status: UYARI
Air raw: 820 quality: 80% status: TEHLIKE
```

Monitor'den cikmak icin genelde `Ctrl+C` yeterlidir.

## VS Code Gorevleri

Terminal komutlarini elle yazmak istemezsen workspace gorevleri zaten hazir:

- `Arduino: Build Uno`
- `Arduino: Upload Uno`
- `Arduino: Monitor`

`Upload` ve `Monitor` gorevleri portu sorar. Ayni isleri terminal komutlarinin paketlenmis hali olarak dusunebilirsin.

## Moduller Nasil Bulunur

Yeni gelen biri icin en hizli rota su:

1. `ArduinoPlayground.ino` dosyasina bak.
2. Hangi modulu include ettigini gor.
3. Ilgili `src/<ozellik>/README.md` dosyasini oku.
4. Sonra ayni klasordeki `.h` ve `.cpp` dosyalarina gir.

Bu sayede hangi ozelligin aktif oldugunu ve mantigin nerede yasadigini hizli bulursun.

## Notlar

- Farkli kart kullanirsan `--fqbn` degerini degistirmen gerekir.
- LCD hic veri gostermiyorsa ilk kontrol edilmesi gereken adresler `0x27` ve `0x3F` olur.
- `src/` altindaki moduller Arduino CLI tarafinda derlemeye dahil edilir; bu yuzden ozellikleri klasorleyip duzenli tutmak kolaydir.
