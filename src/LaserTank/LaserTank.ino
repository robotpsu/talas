/*
 * Laser Tank
 */

#include <LaserTank.h>

//#define USE_SW_SERIAL 1

const String tankName = "Laser tank 1";

// Arduino pins
const byte
  pinIn1 = 2,
  pinIn2 = 3,
  pinIn3 = 4,
  pinIn4 = 7,
  pinEn1 = 5,
  pinEn2 = 6,
  pinLaser = 8,
  pinH = 9,
  pinV = 10,
#ifdef USE_SW_SERIAL
  pinRx = 11,
  pinTx = 12,
#endif
  pinDead = A0,
  pinLife1 = A1,
  pinLife2 = A2,
  pinLife3 = A3;

#ifdef USE_SW_SERIAL
SoftwareSerial bt(pinRx, pinTx);
# define TYPE SoftwareSerial
# define BT bt
#else
# define TYPE HardwareSerial
# ifdef SERIAL_PORT_HARDWARE_OPEN
#  define BT SERIAL_PORT_HARDWARE_OPEN
# else
#  define BT SERIAL_PORT_HARDWARE
# endif
#endif
LaserTank<TYPE> tank(tankName);

void setup() {
  tank.attachDriver(pinIn1, pinIn2, pinIn3, pinIn4, pinEn1, pinEn2);
  tank.attachTurret(pinLaser);
  tank.attachHealth(pinDead, pinLife1, pinLife2, pinLife3);
  tank.attachBluetooth(&BT);

  tank.setup();
}

void loop() {
  tank.loop();
}

/* vim: set ft=arduino et sw=2 ts=2: */

