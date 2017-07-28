#ifndef LaserBasin_h
#define LaserBasin_h

#include <Arduino.h>
#include "Turret.h"

#define SERIAL_SPEED 9600

#define BT_SPEED 38400 // baud
#define BT_TIMEOUT 100 // ms
#define BT_STR_TERM '\n'

#define BASIN_MAX_NAME_LENGTH  24 // characters
#define BASIN_MAX_HEALTH        3 // 1..3
#define BASIN_START_SPEED     127 // 0..255
#define BASIN_DISPATCH_DELAY   10 // ms

class LaserBasin
{
public:
  // Constructor
  LaserBasin(String);

  // Set Arduino board pins
  void attachDriver(byte, byte, byte, byte);
  void attachDriver(byte, byte, byte, byte, byte, byte);
  void attachTurret(byte);
  void attachTurret(byte, byte, byte);
  void attachHealth(byte, byte, byte, byte);

  // Get data
  byte getHealth();
  String getName();
  byte getTurretH();  // Turret horizontal angle
  byte getTurretV();  // Turret vertical angle

  // Set data
  void setName(String);
  void setStartSpeed(byte);
  void setTurretRange(byte, byte, byte, byte);
  void setTurretDelta(byte, byte);

  // Reset health
  void resetHealth();

  // Reset turret
  void resetTurret();

  // Move
  void forward();
  void backward();
  void left();
  void right();
  // Stop
  void stop();
  // Control speed
  void setSpeed(byte);
  void slower();
  void faster();

  // Rotate turret
  void turretUp();
  void turretDown();
  void turretLeft();
  void turretRight();
  // Fire laser gun
  void fire();
  // Handle hit
  void hit();

  // Initial setup
  void setup();
  // Dispatch commands
  void loop();

private:
  String _name;
  volatile byte _health;  // 0..3

  // L289N driver input pins
  byte _pinRight1, _pinRight2, _pinLeft1, _pinLeft2;
  // L289N driver enable pins
  byte _pinRightEn, _pinLeftEn;
  // Health indicator (LED) control pins
  byte _pinDead, _pinLife1, _pinLife2, _pinLife3;

  // Drives speed
  byte _speedRight, _speedLeft;

  Turret _turret;

  void move(byte);

  void showHealth();

#if defined(SERIAL_PORT_HARDWARE_OPEN) && defined(USE_AT_MODE)
  static const unsigned long _rates[];
  static const size_t _numRates;
  static const char *_cmds[];
  static const size_t _numCmds;
  // HC-05 AT-commands mode speed
  unsigned long _atMode;
  // AT-commands mode functions
  unsigned long inCmdMode();
  void printBtInfo();
#endif
};

#endif
/* vim: set ft=arduino et sw=2 ts=2: */

