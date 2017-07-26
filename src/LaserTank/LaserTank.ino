/*
 * Laser Tank
 */

#include <LaserTank.h>

#define TANK_NAME "Laser tank 1"

// Driver
#define PIN_IN1 2
#define PIN_IN2 3
#define PIN_IN3 4
#define PIN_IN4 7
#define PIN_EN1 5
#define PIN_EN2 6
// Turret
#define PIN_LASER  8
#define PIN_H      9
#define PIN_V     10
// Health indicator
#define PIN_DEAD  A0
#define PIN_LIFE1 A1
#define PIN_LIFE2 A2
#define PIN_LIFE3 A3

LaserTank tank(TANK_NAME);

void setup() {
  tank.attachDriver(PIN_IN1, PIN_IN2, PIN_IN3, PIN_IN4, PIN_EN1, PIN_EN2);
  tank.attachTurret(PIN_LASER);
  tank.attachHealth(PIN_DEAD, PIN_LIFE1, PIN_LIFE2, PIN_LIFE3);

  tank.setup();
}

void loop() {
  tank.loop();
}

/* vim: set ft=arduino et sw=2 ts=2: */

