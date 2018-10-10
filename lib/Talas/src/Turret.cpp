/**
 * \file
 * \brief   Turret class implementation.
 * \author  Vasiliy Polyakov <vp@psu.ru>
 * \author  RobotPSU https://robot.psu.ru/
 * \date    2017-2018
 */

#include "Turret.h"

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

void Turret::setRange(uint8_t minH, uint8_t maxH, uint8_t minV, uint8_t maxV) {
  _minH = minH; _maxH = maxH;
  _minV = minV; _maxV = maxV;
}

void Turret::setDelta(uint8_t dH, uint8_t dV) {
  _dH = dH;
  _dV = dV;
}

void Turret::attach(uint8_t pinLaser) {
  pinMode(_pinLaser = pinLaser, OUTPUT);
}

void Turret::attach(uint8_t pinLaser, uint8_t pinH, uint8_t pinV) {
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

uint8_t Turret::getH() {
  return _servoH.read();
}

uint8_t Turret::getV() {
  return _servoV.read();
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
  if (_enabled && _servoH.attached() && _h < _maxH)
    _servoH.write(_h += _dH);
}

void Turret::right() {
  if (_enabled && _servoH.attached() && _h > _minH)
    _servoH.write(_h -= _dH);
}

void Turret::impulse(unsigned long length) {
  digitalWrite(_pinLaser, HIGH);
  delay(length);
  digitalWrite(_pinLaser, LOW);
}

/* vim: set ft=arduino et sw=2 ts=2: */
