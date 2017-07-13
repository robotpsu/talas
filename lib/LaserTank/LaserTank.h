#ifndef LASERTANK_H
#define LASERTANK_H

#include <Arduino.h>
#include <Servo.h>

#define BT_STRING_TERMINATOR '\n'

#define TANK_MAX_NAME_LENGTH 24
#define TANK_MAX_HEALTH 3

#define TURRET_START_H 0
#define TURRET_START_V 0
#define TURRET_MIN_H 0
#define TURRET_MAX_H 180
#define TURRET_MIN_V 0
#define TURRET_MAX_V 90
#define TURRET_DELTA_H 1
#define TURRET_DELTA_V 1

class LaserTank
{
  public:
    static const char stringTerminator;

    // Constructor
    LaserTank(String);

    // Set Arduino board pins
    void setDriverPins(byte, byte, byte, byte);
    void setSpeedPins(byte, byte);
    void setTurretPins(byte, byte);
    void setLaserPin(byte);
    void setHealthPins(byte, byte, byte, byte);

    // Get tank data
    static byte getMaxHealth();
    byte getHealth();
    String getName();

    // Set tank data
    void setName(String);

    // Reset health
    void reset();

    // Dispatch commands
    void dispatch(byte);

  private:
    static const byte maxHealth;
    static const byte minAngleH, maxAngleH;
    static const byte minAngleV, maxAngleV;
    static const byte deltaH, deltaV;

    byte health;
    String name;

    // L289N driver input pins
    byte pinIn1, pinIn2, pinIn3, pinIn4;
    // L289N driver enable pins
    byte pinEn1, pinEn2;
    // Turret rotation control pins
    byte pinTurretH, pinTurretV;
    // Laser control pin
    byte pinLaser;
    // Health indicator (LEDs) control pins
    byte pinDead, pinLife1, pinLife2, pinLife3;

    Servo turretH, turretV;
    byte angleH, angleV;

    // Tank movement
    void forward();
    void backward();
    void left();
    void right();
    void stop();

    // Turret control
    void turretUp();
    void turretDown();
    void turretLeft();
    void turretRight();
    void turretStop();

    void fire();

    // Handle hit
    void hit();
    void showHealth();
};

#endif
/* vim: set ft=arduino et sw=2 ts=2: */

