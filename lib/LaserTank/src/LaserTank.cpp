/*
 * LaserTank.cpp - LaserTank class implementation
 */

#include "LaserTank.h"

#ifdef SERIAL_PORT_HARDWARE_OPEN
# define BT SERIAL_PORT_HARDWARE_OPEN

// HC-05 baud rates
const unsigned long LaserTank::_rates[] = {
    4800, 9600, 19200, 38400, 57600, 115200,
    23400, 460800, 921600, 1382400};
const size_t LaserTank::_numRates = sizeof(LaserTank::_rates) / sizeof(unsigned long);

// HC-05 AT-commands to get some info
const char *LaserTank::_cmds[] = {
  "NAME", "VERSION", "UART", "ADDR", "PSWD", "ROLE", "CMODE", "CLASS", "IAC"};
const size_t LaserTank::_numCmds = sizeof(LaserTank::_cmds) / sizeof(char*);

unsigned long LaserTank::inCmdMode() {
  char buf[32];

  for(size_t i = 0; i < _numRates; i++) {
    BT.begin(_rates[i]);
    BT.setTimeout(100);
    BT.flush();
    BT.write("AT\r\n");
    if (BT.readBytes(buf, 32) > 0)
      return _atMode = _rates[i];
    BT.end();
  }

  return _atMode = 0;
}

void LaserTank::printBtInfo() {
  for (size_t i = 0; i < _numCmds; i++) {
    BT.write("AT+"); BT.write(_cmds[i]); BT.write("?\r\n");
    String s = BT.readString();
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
#else
# define BT SERIAL_PORT_HARDWARE
#endif

LaserTank::LaserTank(String n) : _name(n) {
  _pinRight1 = _pinRight2 = _pinLeft1 = _pinLeft2 = _pinRightEn = _pinLeftEn
    = _pinDead = _pinLife1 = _pinLife2 = _pinLife3 = 0;
  _health = TANK_MAX_HEALTH;
  _speedRight = _speedLeft = TANK_START_SPEED;
}

void LaserTank::setup() {
  resetHealth();
  resetTurret();

#ifdef SERIAL_PORT_HARDWARE_OPEN
  if (unsigned long rate = inCmdMode()) {
    SERIAL_PORT_MONITOR.begin(SERIAL_SPEED);
    SERIAL_PORT_MONITOR.print("Laser tank «");
    SERIAL_PORT_MONITOR.print(_name);
    SERIAL_PORT_MONITOR.println("» configuration");
    SERIAL_PORT_MONITOR.println("\nBluetooth module info:");
    printBtInfo();
    SERIAL_PORT_MONITOR.println("\nEnter commands:");
    return;
  }
#endif
  BT.begin(BT_SPEED);
  BT.setTimeout(BT_TIMEOUT);
}

void LaserTank::attachDriver(byte pinIn1, byte pinIn2, byte pinIn3, byte pinIn4) {
  pinMode(_pinRight1 = pinIn1, OUTPUT);
  pinMode(_pinRight2 = pinIn2, OUTPUT);
  pinMode(_pinLeft1  = pinIn3, OUTPUT);
  pinMode(_pinLeft2  = pinIn4, OUTPUT);
  _pinRightEn = _pinLeftEn = 0;
}

void LaserTank::attachDriver(
    byte pinIn1, byte pinIn2, byte pinIn3, byte pinIn4,
    byte pinEn1, byte pinEn2) {
  attachDriver(pinIn1, pinIn2, pinIn3, pinIn4);
  pinMode(_pinRightEn = pinEn1, OUTPUT);
  pinMode(_pinLeftEn  = pinEn2, OUTPUT);
}

void LaserTank::attachTurret(byte pinLaser) {
  _turret.attach(pinLaser);
}

void LaserTank::attachTurret(byte pinLaser, byte pinH, byte pinV) {
  _turret.attach(pinLaser, pinH, pinV);
}

void LaserTank::attachHealth(byte pinDead, byte pinLife1, byte pinLife2, byte pinLife3) {
  pinMode(_pinDead  = pinDead,  OUTPUT);
  pinMode(_pinLife1 = pinLife1, OUTPUT);
  pinMode(_pinLife2 = pinLife2, OUTPUT);
  pinMode(_pinLife3 = pinLife3, OUTPUT);
}

byte LaserTank::getHealth() {
  return _health;
}

String LaserTank::getName() {
  return _name;
}

void LaserTank::setName(String n) {
  _name = n.substring(0, TANK_MAX_NAME_LENGTH);
}

void LaserTank::setTurretRange(byte minH, byte maxH, byte minV, byte maxV) {
  _turret.setRange(minH, maxH, minV, maxV);
}

void LaserTank::setTurretDelta(byte dH, byte dV) {
  _turret.setDelta(dH, dV);
}

byte LaserTank::getTurretH() {
  return _turret.getH();
}

byte LaserTank::getTurretV() {
  return _turret.getV();
}

void LaserTank::move(byte direction) {
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

void LaserTank::forward() {
  move(0b0101);
}

void LaserTank::backward() {
  move(0b1010);
}

void LaserTank::left() {
  move(0b1001);
}

void LaserTank::right() {
  move(0b0110);
}

void LaserTank::stop() {
  move(0);
}

void LaserTank::faster() {
  _speedRight++;
  _speedLeft++;
}

void LaserTank::slower() {
  _speedRight--;
  _speedLeft--;
}

void LaserTank::turretUp() {
  _turret.up();
}

void LaserTank::turretDown() {
  _turret.down();
}

void LaserTank::turretLeft() {
  _turret.left();
}

void LaserTank::turretRight() {
  _turret.right();
}

void LaserTank::fire() {
  _turret.fire();
}

void LaserTank::hit() {
  if (!_health) return;

  if (--_health) {
    _turret.enable();
  } else {
    _turret.disable();
  }
  showHealth();
}

void LaserTank::showHealth() {
  if (!_pinDead) return;

  digitalWrite(_pinLife3, _health >= 3);
  digitalWrite(_pinLife2, _health >= 2);
  digitalWrite(_pinLife1, _health >= 1);
  digitalWrite(_pinDead, !_health);
}

void LaserTank::resetHealth() {
  _health = TANK_MAX_HEALTH;
  showHealth();
  _turret.enable();
}

void LaserTank::resetTurret() {
  _turret.reset();
}

void LaserTank::loop() {
#ifdef SERIAL_PORT_HARDWARE_OPEN
  if (_atMode) {
    if (BT.available())
      SERIAL_PORT_MONITOR.write(BT.read());
    if (SERIAL_PORT_MONITOR.available()) {
      int b = SERIAL_PORT_MONITOR.read();
      SERIAL_PORT_MONITOR.write(b);
      BT.write(b);
    }
    return;
  }
#endif
  if (BT.available()) {
    switch (BT.read()) {
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
      case 'M': BT.write(TANK_MAX_HEALTH); break;
      case 'H': BT.write(_health); break;
      case 'N': BT.print(_name); BT.print(BT_STR_TERM); break;
      case '/': BT.write(_turret.getH()); BT.write(_turret.getV()); break;

      // Rename tank
      case 'n': setName(BT.readStringUntil(BT_STR_TERM)); break;
    }
    BT.flush();
  }
  delay(TANK_DISPATCH_DELAY);
}

/* vim: ft=arduino et sw=2 ts=2: */
