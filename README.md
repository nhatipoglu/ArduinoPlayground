# Demo

Bu proje, HC-SR04 ultrasonik mesafe sensoru ile olcum yapar, nesne yaklastikca buzzer daha sik bipler ve I2C 16x2 LCD ekranda mesafe/durum gosterir.

## Davranis

- `10 cm` ustunde buzzer susar.
- `10 cm` altinda buzzer aralikli bipler.
- Mesafe azaldikca bip araligi kisalir.
- Olcum alinamazsa seri porta `No measurement` yazar.
- Olcum varsa seri porta `Distance: <deger> cm` yazar.
- LCD ilk satirda mesafeyi, ikinci satirda durum bilgisini gosterir.

## Pin Baglantilari

Kod icindeki pinler:

- `trigPin = 11`
- `echoPin = 12`
- `buzzerPin = 8`
- `lcdAddress = 0x27`

Ornek baglanti:

- HC-SR04 `VCC` -> Arduino `5V`
- HC-SR04 `GND` -> Arduino `GND`
- HC-SR04 `TRIG` -> Arduino `D11`
- HC-SR04 `ECHO` -> Arduino `D12`
- Buzzer `+` -> Arduino `D8`
- Buzzer `-` -> Arduino `GND`
- I2C LCD `GND` -> Arduino `GND`
- I2C LCD `VCC` -> Arduino `5V`
- I2C LCD `SDA` -> Arduino Uno `A4` veya kart uzerindeki `SDA`
- I2C LCD `SCL` -> Arduino Uno `A5` veya kart uzerindeki `SCL`

Not: Arduino Uno'da I2C hatti `D1` ve `D2` degil, `A4/A5` pinleridir. LCD'yi `1` ve `2` numarali dijital pinlere bagladiysan ekranda veri gelmez.

## Gereksinimler

- Arduino CLI kurulu olmali
- Arduino AVR core kurulu olmali
- Hedef kart bu proje icin `Arduino Uno`

Kurulu cekirdekleri gormek icin:

```bash
arduino-cli core list
```

## Derleme

Proje klasorunde su komutu calistir:

```bash
arduino-cli compile --fqbn arduino:avr:uno --build-path "$PWD/build" "$PWD"
```

Basarili olursa derleme boyutu ve RAM kullanimi terminalde gorunur.
Derleme ciktilari proje icindeki `build/` klasorune yazilir.

Baslica build dosyalari:

- `build/Demo.ino.hex`: karta yuklenen ana firmware cikti dosyasi
- `build/Demo.ino.elf`: sembol bilgisi iceren derleme cikti dosyasi
- `build/Demo.ino.with_bootloader.hex`: bootloader dahil hex cikti

## Karti Bulma

Bagli kartlari gormek icin:

```bash
arduino-cli board list
```

Bu komut port bilgisini verir. Ornek portlar:

- Linux: `/dev/ttyUSB0`
- Linux: `/dev/ttyACM0`

## Karta Yukleme

Portu ogrendikten sonra yukleme komutu:

```bash
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno --input-dir "$PWD/build" "$PWD"
```

Port farkliysa `/dev/ttyUSB0` kismini kendi portunla degistir.

## Seri Monitor

Kod `9600` baud ile seri porta mesafe bilgisini yaziyor. I2C LCD takili olmasa bile seri monitor ile izlemek icin:

```bash
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=9600
```

Ornek cikti:

```text
Distance: 8.42 cm
Distance: 6.91 cm
Distance: 4.33 cm
```

## Dosyalar

- `Demo.ino`: ana Arduino sketch giris noktasi, I2C LCD baslatma ve ekran guncelleme burada
- `DistanceAlert.h`: `DistanceAlert` sinifinin bildirimi
- `DistanceAlert.cpp`: sensorden olcum alma ve buzzer davranisinin asil mantigi
- `.vscode/c_cpp_properties.json`: bu workspace icin IntelliSense ayarlari
- `.vscode/tasks.json`: build, upload ve monitor gorevleri
- `build/`: derleme ciktilarinin yazildigi klasor

## Kod Yapisi

Proje artik tek dosya yerine moduler yapida:

- `Demo.ino` sadece sistemi baslatir, I2C LCD'yi hazirlar ve dongude `distanceAlert.update()` cagirir.
- `DistanceAlert.h` sinif arayuzunu tutar.
- `DistanceAlert.cpp` `measurePulseDuration()`, `silenceBuzzer()` ve `beepByDistance()` gibi metotlarla asil davranisi uygular.

Bu yapi kod buyudukce yeni sensor veya alarm mantiklarini ayirmayi kolaylastirir.

## VS Code Gorevleri

VS Code icinden `Run Task` ile su gorevleri calistirabilirsin:

- `Arduino: Build Uno`
- `Arduino: Upload Uno`
- `Arduino: Monitor`

`Upload` ve `Monitor` gorevleri port bilgisini sorar. Varsayilan olarak `/dev/ttyUSB0` gelir.

Tipik akisi su sekilde kullanabilirsin:

1. `Arduino: Build Uno`
2. `Arduino: Upload Uno`
3. `Arduino: Monitor`

## Notlar

- Farkli Arduino kart kullanacaksan `--fqbn` degerini degistirmen gerekir.
- Farkli kart kullanirsan `.vscode/c_cpp_properties.json` icindeki IntelliSense ayarlarini da karta gore guncellemek gerekebilir.
- Buzzer davranisini degistirmek icin `DistanceAlert.cpp` icindeki `alertDistanceCm_` ve `beepDurationMs_` degerlerini ayarlayabilirsin.
- LCD hic yazmiyorsa yaygin I2C adresleri `0x27` ve `0x3F` olur; gerekiyorsa `Demo.ino` icindeki `lcdAddress` degerini degistir.
- Klon kartlarda `arduino-cli board list` ciktisinda kart bazen `Unknown` gorunebilir; port dogruysa `arduino:avr:uno` ile yukleme yine calisabilir.
