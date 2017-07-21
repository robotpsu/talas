/*
 * Laser Tank
 */

#include "LaserTank.h"

const String tankName = "СЛУ-01";

const byte PIN_LED = 13;

LaserTank tank(tankName);

void setup() {
  tank.attachDriver(2, 3, 4, 5);
  tank.attachTurret(PIN_LED);
  tank.attachHealth(A0, A1, A2, A3);
  tank.attachBluetooth();

  tank.resetHealth();
  tank.resetTurret();
}

void loop() {
  tank.dispatch();
}

/* vim: set ft=arduino et sw=2 ts=2: */

