#ifndef LASERTANK_H
#define LASERTANK_H

#include <Arduino.h>
#include <Servo.h>

#define BT_STRING_TERMINATOR '\n'

#define TANK_MAX_NAME_LENGTH 24
#define TANK_MAX_HEALTH 3

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
    static const byte h0, v0;

    Servo servoH, servoV;

    byte pinLaser, pinH, pinV;

    boolean enabled;

    byte h, v;
    byte dH, dV;
    byte minH, maxH;
    byte minV, maxV;

    void impulse(unsigned long);
};

class LaserTank
{
  public:
    // Constructor
    LaserTank(String);

    // Set Arduino board pins
    void attachDriver(byte, byte, byte, byte);
    void attachDriver(byte, byte, byte, byte, byte, byte);
    void attachTurret(byte);
    void attachTurret(byte, byte, byte);
    void attachHealth(byte, byte, byte, byte);

    // Get tank data
    static byte getMaxHealth();
    byte getHealth();
    String getName();

    // Set tank data
    void setName(String);
    void setTurretRange(byte, byte, byte, byte);
    void setTurretDelta(byte, byte);

    // Reset health
    void resetHealth();

    // Reset turret
    void resetTurret();

    // Dispatch commands
    void dispatch(byte);

  private:
    static const char stringTerminator;
    static const byte maxHealth;

    // L289N driver input pins
    byte pinIn1, pinIn2, pinIn3, pinIn4;
    // L289N driver enable pins
    byte pinEn1, pinEn2;
    // Health indicator (LEDs) control pins
    byte pinDead, pinLife1, pinLife2, pinLife3;

    String name;
    byte health;

    Turret turret;

    // Tank movement
    void forward();
    void backward();
    void left();
    void right();
    void stop();

    // Handle hit
    void hit();
    void showHealth();
};

#endif
/* vim: set ft=arduino et sw=2 ts=2: */

