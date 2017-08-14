# ТАЛАС

Тазик лазерный самоходный (ТАЛАС).

## Файловая структура

    lib/
        Talas/                  — библиотека для управления ТАЛАС
            src/                — исходный код библиотеки
                Talas.h         — заголовочный файл библиотеки
                Talas.cpp       — класс Talas
                Turret.h        — заголовочный файл для класса Turret
                Turret.cpp      — класс Turret
            library.properties  — метаданные библиотеки
            keywords.txt        — ключевые слова для подсветки синтаксиса
        Servo/                  — библиотека для управления сервоприводами
    src/Talas/
        Talas.ino               — скетч для Arduino
        Talas.fz                — скетч для Fritzing
    android/
        talas.aia               — проект MIT App Inventor 2 (приложение для Android)
    pcb/                        — печатные платы
    resources/                  — ресурсы для приложений (картинки, звуки и т. п.)
    docs/                       — документация
    Doxyfile                    — настройки Doxygen
    README.md                   — описание проекта

## Команды

ТАЛАС управляется по Bluetooth с помощью следующих команд:

* движение:
	* `w` — вперёд;
	* `s` — назад;
	* `a` — поворот влево;
	* `d` — поворот вправо;
	* `x` — останов;
* скорость:
	* `+` — быстрее;
	* `-` — медленнее;
	* `^` — установить скорость (1-byte unsigned integer);
	* `V` — передать скорость (1-byte unsigned integer);
* турель:
	* `8` — поворот вверх;
	* `2` — поворот вниз;
	* `4` — поворот влево;
	* `6` — поворот вправо;
	* `0` — поворот в исходную позицию;
	* `/` — передать положение турели — 2 угла (2 1-byte unsigned integers);
* лазер:
	* `5` — огонь;
* состояние:
	* `M` — передать максимальное число жизней (1-byte unsigned integer);
	* `H` — передать оставшееся число жизней (текущее «здоровье») (1-byte unsigned integer);
	* `R` — восстановить «здоровье;
	* `*` — имитировать попадание;
* название:
	* `N` — передать название ('\n'-terminated string);
	* `n` — принять новое название и переименовать ТАЛАС ('\n'-terminated string).

## Библиотека

### Установка

Установить библиотеку Talas можно с помощью команды меню `Скетч` → `Подключить библиотеку` → `Добавить .ZIP библиотеку…`.
В появившемся диалоговом окне нужно выбрать директорию `lib/Talas` и нажать `Open` (`Открыть`).
Библиотека установлена — можно собирать и загружать скетч.

### Документация

* [Описание API](https://vpsu.github.io/talas/)

## Arduino

Библиотека и скетч протестированы на [Arduino Uno Rev3](https://store.arduino.cc/arduino-uno-rev3) и [Arduino Mega 2560 Rev3](https://store.arduino.cc/arduino-mega-2560-rev3).

### Arduino Uno

`Serial`

### Arduino Mega

`Serial1`

## Android

Для создания приложения для Android использован [MIT App Inventor 2](http://appinventor.mit.edu/explore/).

### Иконки

* ![НЛО](resources/ufo-16.png "НЛО") — автор: [Pixel Buddha](https://www.flaticon.com/authors/pixel-buddha), лицензия: [CC 3.0 BY](http://creativecommons.org/licenses/by/3.0/ "Creative Commons BY 3.0")
* [Material Design](https://material.io/icons/)

### Ошибки

* Bluetooth.Connect: 507: Unable to connect. Is the device turned on?
* Bluetooth.SendText: 515: Not connected to a Bluetooth device.

## Полезные ссылки

* [Arduino Language Reference](https://www.arduino.cc/en/Reference/HomePage)
* [Arduino.h](https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Arduino.h)
* [Arduino Interrupts](http://gammon.com.au/interrupts)
* [Arduino 101: Timers and Interrupts](http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts)
* [Photocell Tutorial](http://www.instructables.com/id/Photocell-tutorial/)

### Datasheets

* [ATmega2560](http://www.atmel.com/Images/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf)
* [L298 Dual Full-Bridge Driver](https://www.sparkfun.com/datasheets/Robotics/L298_H_Bridge.pdf)
