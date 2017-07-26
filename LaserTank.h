#ifndef LASERTANK_H
#define LASERTANK_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Turret.h"

#define USE_SW_SERIAL 1

#define BT_SPEED 38400 // baud
#define BT_TIMEOUT 100 // ms
#define BT_STRING_TERMINATOR '\n'

#define TANK_MAX_NAME_LENGTH  24 // characters
#define TANK_MAX_HEALTH        3 // 1..3
#define TANK_START_SPEED     127 // 0..255
#define TANK_DISPATCH_DELAY   10 // ms

class LaserTank
{
  public:
    // Constructor
    LaserTank(String);
    LaserTank(String, byte, byte);

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
    void dispatch();

  private:
    static const char strTerm;
    static const byte maxHealth;

    String _name;
    byte _health;  // 0..3

    // L289N driver input pins
    byte _pinIn1, _pinIn2, _pinIn3, _pinIn4;
    // L289N driver enable pins
    byte _pinEn1, _pinEn2;
    // Health indicator (LEDs) control pins
    byte _pinDead, _pinLife1, _pinLife2, _pinLife3;

    // Drives speed
    byte _speed1, _speed2;

#ifdef USE_SW_SERIAL
    SoftwareSerial _bt;
#endif
    Turret _turret;

    // Tank movement
    void move(byte);

    void forward();
    void backward();
    void left();
    void right();

    void stop();

    void slower();
    void faster();

    // Handle hit
    void hit();
    void showHealth();
};

#endif
/* vim: set ft=arduino et sw=2 ts=2: */

