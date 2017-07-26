#ifndef LASERTANK_H
#define LASERTANK_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Turret.h"

#define SERIAL_SPEED 9600

#define BT_SPEED 38400 // baud
#define BT_TIMEOUT 100 // ms
#define BT_STR_TERM '\n'

#define TANK_MAX_NAME_LENGTH  24 // characters
#define TANK_MAX_HEALTH        3 // 1..3
#define TANK_START_SPEED     127 // 0..255
#define TANK_DISPATCH_DELAY   10 // ms

template<class S>
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
  void attachBluetooth(S*);

  // Get tank data
  byte getHealth();
  String getName();
  byte getTurretH();  // Turret horizontal angle
  byte getTurretV();  // Turret vertical angle

  // Set tank data
  void setName(String);
  void setStartSpeed(byte);
  void setTurretRange(byte, byte, byte, byte);
  void setTurretDelta(byte, byte);

  // Reset health
  void resetHealth();

  // Reset turret
  void resetTurret();

  // Move tank
  void forward();
  void backward();
  void left();
  void right();
  void stop();
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
  static const unsigned long _rates[];
  static const size_t _numRates;
  static const char *_cmds[];
  static const size_t _numCmds;

  String _name;
  byte _health;  // 0..3

  // HC-05 AT-commands mode speed
  unsigned long _atMode;

  // L289N driver input pins
  byte _pinRight1, _pinRight2, _pinLeft1, _pinLeft2;
  // L289N driver enable pins
  byte _pinRightEn, _pinLeftEn;
  // Health indicator (LED) control pins
  byte _pinDead, _pinLife1, _pinLife2, _pinLife3;

  // Drives speed
  byte _speedRight, _speedLeft;

  S *_bt;
  Turret _turret;

  boolean isSerialOpen();
  unsigned long inCmdMode();
  void printBtInfo();

  void move(byte);

  void showHealth();
};

/**
 * Static properties
 */
// HC-05 baud rates
template<class S>
const unsigned long LaserTank<S>::_rates[] = {
    4800, 9600, 19200, 38400, 57600, 115200,
    23400, 460800, 921600, 1382400};
template<class S>
const size_t LaserTank<S>::_numRates = sizeof(LaserTank<S>::_rates) / sizeof(unsigned long);

// HC-05 AT-commands to get some info
template<class S>
const char *LaserTank<S>::_cmds[] = {
  "NAME", "VERSION", "UART", "ADDR", "PSWD", "ROLE", "CMODE", "CLASS", "IAC"};
template<class S>
const size_t LaserTank<S>::_numCmds = sizeof(LaserTank<S>::_cmds) / sizeof(char*);

template<class S>
LaserTank<S>::LaserTank(String n) : _name(n) {
  _pinRight1 = _pinRight2 = _pinLeft1 = _pinLeft2 = _pinRightEn = _pinLeftEn
    = _pinDead = _pinLife1 = _pinLife2 = _pinLife3 = 0;
  _health = TANK_MAX_HEALTH;
  _speedRight = _speedLeft = TANK_START_SPEED;
  _bt = NULL;
  _atMode = 0;
}

template<class S>
void LaserTank<S>::attachBluetooth(S *bt) {
  _bt = bt;
  if (unsigned long rate = inCmdMode()) {
    SERIAL_PORT_MONITOR.begin(SERIAL_SPEED);
    SERIAL_PORT_MONITOR.print("Laser tank «");
    SERIAL_PORT_MONITOR.print(_name);
    SERIAL_PORT_MONITOR.println("» configuration");
    SERIAL_PORT_MONITOR.println("\nBluetooth module info:");
    printBtInfo();
    SERIAL_PORT_MONITOR.println("\nEnter commands:");
  } else {
    _bt->begin(BT_SPEED);
    _bt->setTimeout(BT_TIMEOUT);
  }
}

template<class S>
void LaserTank<S>::attachDriver(byte pinIn1, byte pinIn2, byte pinIn3, byte pinIn4) {
  pinMode(_pinRight1 = pinIn1, OUTPUT);
  pinMode(_pinRight2 = pinIn2, OUTPUT);
  pinMode(_pinLeft1  = pinIn3, OUTPUT);
  pinMode(_pinLeft2  = pinIn4, OUTPUT);
  _pinRightEn = _pinLeftEn = 0;
}

template<class S>
void LaserTank<S>::attachDriver(
    byte pinIn1, byte pinIn2, byte pinIn3, byte pinIn4,
    byte pinEn1, byte pinEn2) {
  attachDriver(pinIn1, pinIn2, pinIn3, pinIn4);
  pinMode(_pinRightEn = pinEn1, OUTPUT);
  pinMode(_pinLeftEn  = pinEn2, OUTPUT);
}

template<class S>
void LaserTank<S>::attachTurret(byte pinLaser) {
  _turret.attach(pinLaser);
}

template<class S>
void LaserTank<S>::attachTurret(byte pinLaser, byte pinH, byte pinV) {
  _turret.attach(pinLaser, pinH, pinV);
}

template<class S>
void LaserTank<S>::attachHealth(byte pinDead, byte pinLife1, byte pinLife2, byte pinLife3) {
  pinMode(_pinDead  = pinDead,  OUTPUT);
  pinMode(_pinLife1 = pinLife1, OUTPUT);
  pinMode(_pinLife2 = pinLife2, OUTPUT);
  pinMode(_pinLife3 = pinLife3, OUTPUT);
}

template<class S>
byte LaserTank<S>::getHealth() {
  return _health;
}

template<class S>
String LaserTank<S>::getName() {
  return _name;
}

template<class S>
void LaserTank<S>::setName(String n) {
  _name = n.substring(0, TANK_MAX_NAME_LENGTH);
}

template<class S>
void LaserTank<S>::setTurretRange(byte minH, byte maxH, byte minV, byte maxV) {
  _turret.setRange(minH, maxH, minV, maxV);
}

template<class S>
void LaserTank<S>::setTurretDelta(byte dH, byte dV) {
  _turret.setDelta(dH, dV);
}

template<class S>
byte LaserTank<S>::getTurretH() {
  return _turret.getH();
}

template<class S>
byte LaserTank<S>::getTurretV() {
  return _turret.getV();
}

template<class S>
boolean LaserTank<S>::isSerialOpen() {
  return (HardwareSerial*)_bt != &SERIAL_PORT_MONITOR;
}

template<class S>
unsigned long LaserTank<S>::inCmdMode() {
  char buf[32];

  for(size_t i = 0; i < _numRates; i++) {
    _bt->begin(_rates[i]);
    _bt->setTimeout(100);
    _bt->flush();
    _bt->write("AT\r\n");
    if (_bt->readBytes(buf, 32) > 0)
      return _atMode = _rates[i];
    _bt->end();
  }

  return _atMode = 0;
}

template<class S>
void LaserTank<S>::printBtInfo() {
  for (size_t i = 0; i < _numCmds; i++) {
    _bt->write("AT+"); _bt->write(_cmds[i]); _bt->write("?\r\n");
    String s = _bt->readString();
    if (!s.length()) continue;
    if (s.endsWith("OK\r\n")) {
      s.remove(s.indexOf('\r'));
      SERIAL_PORT_MONITOR.println(s);
    } else {
      SERIAL_PORT_MONITOR.write('+');
      SERIAL_PORT_MONITOR.print(_cmds[i]);
      SERIAL_PORT_MONITOR.print(":FAIL");
    }
  }
}

template<class S>
void LaserTank<S>::move(byte direction) {
  if (!_health || !_pinRight1 || !_pinLeft1) return;

  digitalWrite(_pinRight1, bitRead(direction, 0));
  digitalWrite(_pinRight2, bitRead(direction, 1));
  digitalWrite(_pinLeft1,  bitRead(direction, 2));
  digitalWrite(_pinLeft2,  bitRead(direction, 3));

  if (_pinRightEn)
    analogWrite(_pinRightEn, direction ? _speedRight : 0);
  if (_pinLeftEn)
    analogWrite(_pinLeftEn,  direction ? _speedLeft  : 0);
}

template<class S>
void LaserTank<S>::forward() {
  move(0b0101);
}

template<class S>
void LaserTank<S>::backward() {
  move(0b1010);
}

template<class S>
void LaserTank<S>::left() {
  move(0b1001);
}

template<class S>
void LaserTank<S>::right() {
  move(0b0110);
}

template<class S>
void LaserTank<S>::stop() {
  move(0);
}

template<class S>
void LaserTank<S>::faster() {
  _speedRight++;
  _speedLeft++;
}

template<class S>
void LaserTank<S>::slower() {
  _speedRight--;
  _speedLeft--;
}

template<class S>
void LaserTank<S>::turretUp() {
  _turret.up();
}

template<class S>
void LaserTank<S>::turretDown() {
  _turret.down();
}

template<class S>
void LaserTank<S>::turretLeft() {
  _turret.left();
}

template<class S>
void LaserTank<S>::turretRight() {
  _turret.right();
}

template<class S>
void LaserTank<S>::fire() {
  _turret.fire();
}

template<class S>
void LaserTank<S>::hit() {
  if (!_health) return;

  if (--_health) {
    _turret.enable();
  } else {
    _turret.disable();
  }
  showHealth();
}

template<class S>
void LaserTank<S>::showHealth() {
  if (!_pinDead) return;

  digitalWrite(_pinLife3, _health >= 3);
  digitalWrite(_pinLife2, _health >= 2);
  digitalWrite(_pinLife1, _health >= 1);
  digitalWrite(_pinDead, !_health);
}

template<class S>
void LaserTank<S>::resetHealth() {
  _health = TANK_MAX_HEALTH;
  showHealth();
  _turret.enable();
}

template<class S>
void LaserTank<S>::resetTurret() {
  _turret.reset();
}

template<class S>
void LaserTank<S>::setup() {
  resetHealth();
  resetTurret();
}

template<class S>
void LaserTank<S>::loop() {
  if (!_bt) return;

  if (_atMode) {
  	if (isSerialOpen()) {
      if (_bt->available())
        SERIAL_PORT_MONITOR.write(_bt->read());
      if (SERIAL_PORT_MONITOR.available()) {
        int b = SERIAL_PORT_MONITOR.read();
        SERIAL_PORT_MONITOR.write(b);
        _bt->write(b);
      }
    }
  } else {
    if (_bt->available()) {
      switch (_bt->read()) {
        // Move tank
        case 'w': forward();  break;
        case 'a': left();     break;
        case 's': backward(); break;
        case 'd': right();    break;

        // Stop tank
        case 'x': stop(); break;

        // Control speed
        case '-': slower(); break;
        case '+': faster(); break;

        // Rotate turret
        case '8': _turret.up();    break;
        case '6': _turret.right(); break;
        case '4': _turret.left();  break;
        case '2': _turret.down();  break;

        // Move turret to the initial position
        case '0': _turret.reset(); break;

        // Fire laser gun
        case '5': _turret.fire(); break;

        // Simulate hit
        case '*': hit(); break;

        // Reset tank health
        case 'r': resetHealth(); break;

        // Send tank data
        case 'M': _bt->write(TANK_MAX_HEALTH); break;
        case 'H': _bt->write(_health); break;
        case 'N': _bt->print(_name); _bt->print(BT_STR_TERM); break;
        case '/': _bt->write(_turret.getH()); _bt->write(_turret.getV()); break;

        // Rename tank
        case 'n': setName(_bt->readStringUntil(BT_STR_TERM)); break;
      }
      _bt->flush();
    }
    delay(TANK_DISPATCH_DELAY);
  }
}

#endif
/* vim: set ft=arduino et sw=2 ts=2: */

