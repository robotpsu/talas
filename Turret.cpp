/*
 * Tank turret class implementation
 */

#include "LaserTank.h"

Turret::Turret() {
  _pinLaser = 0;

  _h = h0 = TURRET_START_H;
  _v = v0 = TURRET_START_V;

  _dH = TURRET_DELTA_H;
  _dV = TURRET_DELTA_V;

  _minH = TURRET_MIN_H; _maxH = TURRET_MAX_H;
  _minV = TURRET_MIN_V; _maxV = TURRET_MAX_V;

  _enabled = true;
}

void Turret::setRange(byte minH, byte maxH, byte minV, byte maxV) {
  _minH = minH; _maxH = maxH;
  _minV = minV; _maxV = maxV;
}

void Turret::setDelta(byte dH, byte dV) {
  _dH = dH;
  _dV = dV;
}

void Turret::attach(byte pinLaser) {
  pinMode(_pinLaser = pinLaser, OUTPUT);
}

void Turret::attach(byte pinLaser, byte pinH, byte pinV) {
  attach(pinLaser);
  _servoH.attach(pinH);
  _servoV.attach(pinV);
}

void Turret::enable() {
  _enabled = true;
}

void Turret::disable() {
  _enabled = false;
}

byte Turret::getH() {
  return _h;
}

byte Turret::getV() {
  return _v;
}

void Turret::reset() {
  if (_servoH.attached() && _servoV.attached()) {
    _servoH.write(_h = h0);
    _servoV.write(_v = v0);
  }
}

void Turret::up() {
  if (_enabled && _servoV.attached() && _v < _maxV)
    _servoV.write(_v += _dV);
}

void Turret::down() {
  if (_enabled && _servoV.attached() && _v > _minV)
    _servoV.write(_v -= _dV);
}

void Turret::left() {
  if (_enabled && _servoH.attached() && _h > _minH)
    _servoH.write(_h -= _dH);
}

void Turret::right() {
  if (_enabled && _servoH.attached() && _h < _maxH)
    _servoH.write(_h += _dH);
}

void Turret::impulse(unsigned long length) {
  digitalWrite(_pinLaser, HIGH);
  delay(length);
  digitalWrite(_pinLaser, LOW);
}

void Turret::fire() {
  if (!_enabled || !_pinLaser) return;

  impulse(5);
  delay(5);
  impulse(5);
}

/* vim: set ft=arduino et sw=2 ts=2: */

