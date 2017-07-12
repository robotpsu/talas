#ifndef LASERTANK_H
#define LASERTANK_H

#include <Arduino.h>
//#include <Servo.h>

#define TANK_MAX_HEALTH 3

class LaserTank
{
  public:
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

    // Reset health
    void reset();

  private:
    static const byte maxHealth;
    byte health;
    String name;

    byte pin_in1, pin_in2, pin_in3, pin_in4;
    byte pin_en1, pin_en2;
    byte pin_laser;
    byte pin_dead, pin_life1, pin_life2, pin_life3;

    //Servo turret1, turret2;
};

#endif
/* vim: set ft=arduino et sw=2 ts=2: */

