# Laser Tank

## Файловая структура

    lib/LaserTank/    — библиотека для Arduino
        LaserTank.h   — заголовочный файл
        LaserTank.cpp — код библиотеки
        keywords.txt  — ключевые слова для Arduino IDE
    src/LaserTank/
        LaserTank.ino — скетч для Arduino
    ai2/
        LaserTank.aia — проект MIT App Inventor 2 (приложение для Android)
    README.md         — описание проекта

## Команды

* `w` — двигать танк вперёд;
* `s` — двигать танк назад;
* `a` — повернуть танк влево;
* `d` — повернуть танк вправо;
* `x` — остановить танк;
* `8` — повернуть башню вверх (тангаж);
* `2` — повернуть башню вниз;
* `4` — повернуть башню влево (рыскание);
* `6` — повернуть башню вправо;
* `0` — остановить башню;
* `5` — огонь;
* `N` — передать идентификатор танка (0-terminated string);
* `M` — передать максимальное число жизней (1-byte unsigned integer);
* `H` — передать оставшееся число жизней (текущее «здоровье») (1-byte unsigned integer);
* `R` — восстановить «здоровье».

## Arduino

Танк управляется [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3).

Для написания, компиляции и загрузки скетча использована программа [Arduino IDE](https://www.arduino.cc/en/Main/Software#download) (Windows || Linux || MacOS).

Также удобно использовать [Arturo](https://github.com/scottdarch/Arturo) — утилиту командной строки для Linux:

    ano build && ano upload -q

## Android

Для создания приложения для телефона использован [MIT App Inventor 2](http://appinventor.mit.edu/explore/).
