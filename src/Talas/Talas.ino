/*
 * Тазик лазерный самоходный (ТАЛАС).
 */

#include <Talas.h>

#define TALAS_NAME "ТАЛАС 1"

// Bluetooth
//#define PIN_KEY     22
//#define PIN_STATE   24
// Driver
#define PIN_IN1     23
#define PIN_IN2     27
#define PIN_IN3     31
#define PIN_IN4     35
#define PIN_ENA     11 // PWM
#define PIN_ENB     12 // PWM
// Turret
#define PIN_LASER   49
#define PIN_SERVO_H 25
#define PIN_SERVO_V 29
// Health indicator
#define PIN_HEALTH0 33
#define PIN_HEALTH1 37
#define PIN_HEALTH2 41
#define PIN_HEALTH3 45
// Hit detector
#define PIN_HIT    A11 // analog

Talas talas(TALAS_NAME);

void setup() {
  talas.attachHealth(PIN_HEALTH0, PIN_HEALTH1, PIN_HEALTH2, PIN_HEALTH3);
  talas.attachDriver(PIN_IN1, PIN_IN2, PIN_IN3, PIN_IN4);
  talas.attachTurret(PIN_LASER, PIN_SERVO_H, PIN_SERVO_V);
  talas.attachHitDetector(PIN_HIT);
  talas.attachBluetooth();
}

void loop() {
  talas.dispatch();
}

ISR(TIMER3_COMPB_vect) {
  talas.handleImpulse();
}

/* vim: set ft=arduino et sw=2 ts=2: */

