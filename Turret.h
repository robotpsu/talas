#ifndef Turret_h
#define Turret_h

#include <Arduino.h>
#include <Servo.h>

// Turret angles
#define TURRET_START_H 90
#define TURRET_START_V  0
#define TURRET_DELTA_H  1
#define TURRET_DELTA_V  1
#define TURRET_MIN_H   30
#define TURRET_MAX_H  150
#define TURRET_MIN_V    0
#define TURRET_MAX_V   60

class Turret
{
public:
  byte h0, v0;

  // Constructor
  Turret();

  // Set servos angle range
  void setRange(byte, byte, byte, byte);
  // Set angle increment/decrement value
  void setDelta(byte, byte);

  // Attach laser and servos to pins
  void attach(byte);
  void attach(byte, byte, byte);

  // Toggle turret state
  void enable();
  void disable();

  // Reset turret
  void reset();

  // Get current position (in degrees)
  byte getH();
  byte getV();

  // Rotate turret
  void up();
  void down();
  void left();
  void right();

  // Fire laser gun
  void fire();

private:
  boolean _enabled;
  byte _pinLaser;
  Servo _servoH, _servoV;
  byte _h, _v;
  byte _dH, _dV;
  byte _minH, _maxH;
  byte _minV, _maxV;

  void impulse(unsigned long);
};

#endif
/* vim: set ft=arduino et sw=2 ts=2: */

