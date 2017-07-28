/*
 * Laser Basin — тазик лазерный самоходный (ТАЛАС).
 */

#include <LaserBasin.h>

#define BASIN_NAME "ТАЛАС-01"

// Hit detector
// See https://www.arduino.cc/en/Reference/AttachInterrupt
//#define PIN_HIT 2
// Driver
#define PIN_IN1 2
#define PIN_IN2 3
#define PIN_IN3 4
#define PIN_IN4 5
#define PIN_EN1 6 // PWM
#define PIN_EN2 7 // PWM
// Turret
#define PIN_LASER 50
#define PIN_H     34
#define PIN_V     36
// Health indicator
#define PIN_DEAD  23
#define PIN_LIFE1 25
#define PIN_LIFE2 27
#define PIN_LIFE3 29

LaserBasin basin(BASIN_NAME);

void setup() {
  basin.attachDriver(PIN_IN1, PIN_IN2, PIN_IN3, PIN_IN4, PIN_EN1, PIN_EN2);
  basin.attachTurret(PIN_LASER, PIN_H, PIN_V);
  basin.attachHealth(PIN_DEAD, PIN_LIFE1, PIN_LIFE2, PIN_LIFE3);

  basin.setup();

  //pinMode(PIN_HIT, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(PIN_HIT), hit, RISING);
}

void loop() {
  basin.loop();
}

void hit() {
  basin.hit();
}

/* vim: set ft=arduino et sw=2 ts=2: */

