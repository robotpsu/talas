# Laser Tank

## Файловая структура

    lib/
        LaserTank/
            src/
                LaserTank.h    — заголовочный файл библиотеки
                LaserTank.cpp  — класс LaserTank
                Turret.h
                Turret.cpp     — класс Turret
            library.properties — метаданные библиотеки
            keywords.txt       — ключевые слова для подсветки синтаксиса
        Servo/                 — библиотека для управления сервоприводами
    src/
        LaserTank/
            LaserTank.ino      — скетч для Arduino
    android/
        LaserTank.aia          — проект MIT App Inventor 2 (приложение для Android)
    README.md                  — описание проекта

## Команды

Танк управляется по Bluetooth с помощью следующих команд:

* `w` — двигать танк вперёд;
* `s` — двигать танк назад;
* `a` — повернуть танк влево;
* `d` — повернуть танк вправо;
* `x` — остановить танк;
* `+` — ускорение;
* `-` — замедление;
* `^` — установить скорость (1-byte unsigned integer);
* `8` — повернуть турель вверх;
* `2` — повернуть турель вниз;
* `4` — повернуть турель влево;
* `6` — повернуть турель вправо;
* `0` — повернуть турель в исходную позицию;
* `5` — огонь;
* `*` — имитировать попадание;
* `/` — передать положение турели — 2 угла (2 1-byte unsigned integers);
* `V` — передать скорость (1-byte unsigned integer);
* `M` — передать максимальное число жизней (1-byte unsigned integer);
* `H` — передать оставшееся число жизней (текущее «здоровье») (1-byte unsigned integer);
* `N` — передать идентификатор танка ('\n'-terminated string);
* `n` — принять новый идентификатор и переименовать танк ('\n'-terminated string);
* `r` — восстановить «здоровье».

## Скетч

### Arduino IDE

Для написания, компиляции и загрузки скетча использована программа [Arduino IDE](https://www.arduino.cc/en/Main/Software#download) (Windows || Linux || MacOS).

#### Установка библиотеки LaserTank

Установить библиотеку LaserTank можно с помощью команды меню `Скетч` → `Подключить библиотеку` → `Добавить .ZIP библиотеку…`.
В появившемся диалоговом окне нужно выбрать директорию `lib/LaserTank` и нажать `Open` (`Открыть`).
Библиотека установлена, можно собирать и загружать прошивку.

### Arturo

Также удобно использовать [Arturo](https://github.com/scottdarch/Arturo) — утилиту командной строки для Linux:

    ano build && ano upload -q

## Arduino

Скетч протестирован на [Arduino Uno Rev3](https://store.arduino.cc/arduino-uno-rev3) и [Arduino Mega 2560 Rev3](https://store.arduino.cc/arduino-mega-2560-rev3).

### Arduino Uno

`Serial`

### Arduino Mega

`Serial1`

## Android

Для создания приложения для Android использован [MIT App Inventor 2](http://appinventor.mit.edu/explore/).

### Ошибки

* Bluetooth.Connect: 507: Unable to connect. Is the device turned on?
* Bluetooth.SendText: 515: Not connected to a Bluetooth device.

