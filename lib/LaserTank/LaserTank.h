#ifndef LASERTANK_H
#define LASERTANK_H

#include <Arduino.h>
#include <Servo.h>

#define BT_STRING_TERMINATOR '\n'
#define TANK_MAX_NAME_LENGTH 24
#define TANK_MAX_HEALTH 3

class LaserTank
{
  public:
    static const char stringTerminator;

    // Constructor
    LaserTank(String);

    // Set Arduino board pins
    void setDriverPins(byte, byte, byte, byte);
    void setSpeedPins(byte, byte);
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

    byte health;
    String name;

    // L289N driver input pins
    byte pin_in1, pin_in2, pin_in3, pin_in4;
    // L289N driver enable pins
    byte pin_en1, pin_en2;
    // Laser control pin
    byte pin_laser;
    // Health indicator (LEDs) control pins
    byte pin_dead, pin_life1, pin_life2, pin_life3;

    Servo turret1, turret2;

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

