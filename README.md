# ТАЛАС

ТАЛАС — ТАЛАС лазерный самоходный.

## Библиотека

### Установка

Установить библиотеку Talas можно с помощью команды меню `Скетч` → `Подключить библиотеку` → `Добавить .ZIP библиотеку…`.
В появившемся диалоговом окне нужно выбрать директорию `lib/Talas` и нажать `Open` (`Открыть`).
Библиотека установлена — можно собирать и загружать скетч.

### Документация

* [Описание API](https://vpsu.github.io/talas/)

## Arduino

Библиотека и скетч предназначены для выполнения на [Arduino Mega 2560 Rev3](https://store.arduino.cc/arduino-mega-2560-rev3).

## Приложение для Android [![Доступно в Google Play](resources/google-play-badge-small.png "Доступно в Google Play")](https://play.google.com/store/apps/details?id=appinventor.ai_vvp_psu.TALAS&pcampaignid=MKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1 "Доступно в Google Play")

Для создания приложения использован [MIT App Inventor 2](http://appinventor.mit.edu/explore/).

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
	* `^` — установить скорость (**1-byte unsigned integer**);
	* `V` — передать скорость (**1-byte unsigned integer**);
* турель:
	* `8` — поворот вверх;
	* `2` — поворот вниз;
	* `4` — поворот влево;
	* `6` — поворот вправо;
	* `0` — поворот в исходную позицию;
	* `/` — передать положение турели — 2 угла (2 **1-byte unsigned integers**);
* лазер:
	* `5` — огонь;
* состояние:
	* `M` — передать максимальное число жизней (**1-byte unsigned integer**);
	* `H` — передать оставшееся число жизней (текущее «здоровье») (**1-byte unsigned integer**);
	* `R` — восстановить «здоровье;
	* `*` — имитировать попадание;
* название:
	* `N` — передать название (**'\n'-terminated string**);
	* `n` — принять новое название и переименовать ТАЛАС (**'\n'-terminated string**).

## Полезные ссылки

* [Arduino Language Reference](https://www.arduino.cc/en/Reference/HomePage)
* [Arduino.h](https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Arduino.h)
* [Arduino блокнот программиста](http://robocraft.ru/files/books/arduino_notebook_rus_v1-1.pdf)
* [Arduino Interrupts](http://gammon.com.au/interrupts)
* [Arduino 101: Timers and Interrupts](http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts)
* [Photocell Tutorial](http://www.instructables.com/id/Photocell-tutorial/)
* [HC-03/05 Embedded Bluetooth Serial Communication Module AT command set](http://eskimon.fr/wp-content/uploads/2014/10/commandes_AT_HC05.pdf)

### Datasheets

* [ATmega2560](http://www.atmel.com/Images/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf)
* [L298 Dual Full-Bridge Driver](https://www.sparkfun.com/datasheets/Robotics/L298_H_Bridge.pdf)

## Описание файлов

* [`lib/Talas/`](https://github.com/vpsu/talas/blob/master/lib/Talas) — библиотека для управления ТАЛАС
    * [`src/`](https://github.com/vpsu/talas/blob/master/lib/Talas/src) — исходный код библиотеки
        * [`Talas.h`](https://github.com/vpsu/talas/blob/master/lib/Talas/src/Talas.h) — заголовочный файл библиотеки
        * [`Talas.cpp`](https://github.com/vpsu/talas/blob/master/lib/Talas/src/Talas.cpp) — класс [Talas](https://vpsu.github.io/talas/class_talas.html)
        * [`Turret.h`](https://github.com/vpsu/talas/blob/master/lib/Talas/src/Turret.h) — заголовочный файл для класса Turret
        * [`Turret.cpp`](https://github.com/vpsu/talas/blob/master/lib/Talas/src/Turret.cpp) — класс [Turret](https://vpsu.github.io/talas/class_turret.html)
    * [`library.properties`](https://github.com/vpsu/talas/blob/master/lib/Talas/library.properties) — [метаданные](https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification#library-metadata "Library metadata") библиотеки
    * [`keywords.txt`](https://github.com/vpsu/talas/blob/master/lib/Talas/keywords.txt) — [ключевые слова](https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification#keywords "Keywords") для подсветки синтаксиса в [Arduino IDE](https://www.arduino.cc/en/Main/Software#download)
* [`lib/Servo/`](https://github.com/vpsu/talas/blob/master/lib/Servo) — библиотека [Servo](https://github.com/arduino-libraries/Servo) для работы с серводвигателями и шаговыми двигателями ([подмодуль](https://git-scm.com/book/ru/v2/%D0%98%D0%BD%D1%81%D1%82%D1%80%D1%83%D0%BC%D0%B5%D0%BD%D1%82%D1%8B-Git-%D0%9F%D0%BE%D0%B4%D0%BC%D0%BE%D0%B4%D1%83%D0%BB%D0%B8 "Git - Подмодули"))
* [`src/Talas/`](https://github.com/vpsu/talas/blob/master/src/Talas)
    * [`Talas.ino`](https://github.com/vpsu/talas/blob/master/src/Talas/Talas.ino) — скетч для Arduino
    * [`Talas.fz`](https://github.com/vpsu/talas/blob/master/src/Talas/Talas.fz) — скетч для [Fritzing](http://fritzing.org/home/)
* [`android/`](https://github.com/vpsu/talas/blob/master/android)
    * [`talas.aia`](https://github.com/vpsu/talas/blob/master/android/talas.aia) — проект [MIT App Inventor 2](http://appinventor.mit.edu/) (приложение для Android)
* [`pcb/`](https://github.com/vpsu/talas/blob/master/pcb) — печатные платы
* [`resources/`](https://github.com/vpsu/talas/blob/master/resources) — ресурсы для приложений (картинки, звуки и т. п.)
* [`docs/`](https://github.com/vpsu/talas/blob/master/docs) — [документация](https://vpsu.github.io/talas/)
* [`Doxyfile`](https://github.com/vpsu/talas/blob/master/Doxyfile) — настройки [Doxygen](http://www.stack.nl/~dimitri/doxygen/)
* [`README.md`](https://github.com/vpsu/talas/blob/master/README.md) — описание проекта

## Источники и лицензии

* ![НЛО](resources/ufo-16.png "НЛО") — автор: [Pixel Buddha](https://www.flaticon.com/authors/pixel-buddha), лицензия: [CC 3.0 BY](http://creativecommons.org/licenses/by/3.0/ "Creative Commons BY 3.0")
* [Material Design](https://material.io/icons/)
* Google Play и логотип Google Play являются товарными знаками корпорации Google Inc.
