/*
 * Laser tank class implementation
 */

#include "LaserTank.h"

const char LaserTank::strTerm = BT_STRING_TERMINATOR;
const byte LaserTank::maxHealth = TANK_MAX_HEALTH;

byte LaserTank::getMaxHealth() {
  return maxHealth;
}

#ifdef USE_SW_SERIAL
LaserTank::LaserTank(String n, byte rx, byte tx) : _name(n), _bt(rx, tx) {
#else
#define _bt Serial
LaserTank::LaserTank(String n) : _name(n) {
#endif
  _pinIn1 = _pinIn2 = _pinIn3 = _pinIn4 = _pinEn1 = _pinEn2
    = _pinDead = _pinLife1 = _pinLife2 = _pinLife3 = 0;
  _speed1 = _speed2 = TANK_START_SPEED;

  _bt.begin(BT_SPEED);
  _bt.setTimeout(BT_TIMEOUT);
}

void LaserTank::attachDriver(byte in1, byte in2, byte in3, byte in4) {
  pinMode(_pinIn1 = in1, OUTPUT);
  pinMode(_pinIn2 = in2, OUTPUT);
  pinMode(_pinIn3 = in3, OUTPUT);
  pinMode(_pinIn4 = in4, OUTPUT);
  _pinEn1 = _pinEn2 = 0;
}

void LaserTank::attachDriver(byte in1, byte in2, byte in3, byte in4, byte en1, byte en2) {
  attachDriver(in1, in2, in3, in4);
  pinMode(_pinEn1 = en1, OUTPUT);
  pinMode(_pinEn2 = en2, OUTPUT);
}

void LaserTank::attachTurret(byte laser) {
  _turret.attach(laser);
}

void LaserTank::attachTurret(byte laser, byte p0, byte p1) {
  _turret.attach(laser, p0, p1);
}

void LaserTank::attachHealth(byte dead, byte life1, byte life2, byte life3) {
  pinMode(_pinDead  = dead,  OUTPUT);
  pinMode(_pinLife1 = life1, OUTPUT);
  pinMode(_pinLife2 = life2, OUTPUT);
  pinMode(_pinLife3 = life3, OUTPUT);
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

void LaserTank::setTurretRange(byte h0, byte h1, byte v0, byte v1) {
  _turret.setRange(h0, h1, v0 ,v1);
}

void LaserTank::setTurretDelta(byte dh, byte dv) {
  _turret.setDelta(dh, dv);
}

void LaserTank::move(byte direction) {
  if (!_health || !_pinIn1) return;

  digitalWrite(_pinIn1, bitRead(direction, 0));
  digitalWrite(_pinIn2, bitRead(direction, 1));
  digitalWrite(_pinIn3, bitRead(direction, 2));
  digitalWrite(_pinIn4, bitRead(direction, 3));

  if (_pinEn1)
    analogWrite(_pinEn1, direction ? _speed1 : 0);
  if (_pinEn2)
    analogWrite(_pinEn2, direction ? _speed2 : 0);
}

void LaserTank::forward() {
  move(5);
}

void LaserTank::backward() {
  move(10);
}

void LaserTank::left() {
  move(9);
}

void LaserTank::right() {
  move(6);
}

void LaserTank::stop() {
  move(0);
}

void LaserTank::faster() {
  _speed1++;
  _speed2++;
}

void LaserTank::slower() {
  _speed1--;
  _speed2--;
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
  _health = maxHealth;
  showHealth();
  _turret.enable();
}

void LaserTank::resetTurret() {
  _turret.reset();
}

void LaserTank::dispatch() {
  int n = _bt.available();
  int cmd = 0;

  if (n > 0) {
    cmd = _bt.read();
    switch (cmd) {
      // Move tank
      case 'w': forward();  break;
      case 'a': left();     break;
      case 's': backward(); break;
      case 'd': right();    break;

      // Stop tank
      case 'x': stop(); break;

      // Control speed
      case 'j': slower(); break;
      case 'k': faster(); break;

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
      case 'M': _bt.write(maxHealth); break;
      case 'H': _bt.write(_health); break;
      case 'N': _bt.print(_name); _bt.print(strTerm); break;
      case '/': _bt.write(_turret.getH()); _bt.write(_turret.getV()); break;

      // Rename tank
      case 'n': setName(_bt.readStringUntil(strTerm)); break;
    }
    _bt.flush();
  }
  delay(TANK_DISPATCH_DELAY);
}

/* vim: set ft=arduino et sw=2 ts=2: */

