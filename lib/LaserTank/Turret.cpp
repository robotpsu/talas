/*
 * Tank turret class implementation
 */

#include "LaserTank.h"

const byte Turret::h0 = TURRET_START_H;
const byte Turret::v0 = TURRET_START_V;

Turret::Turret() {
  pinLaser = pinH = pinV = 0;

  h = h0;
  v = v0;

  dH = TURRET_DELTA_H;
  dV = TURRET_DELTA_V;

  minH = TURRET_MIN_H; maxH = TURRET_MAX_H;
  minV = TURRET_MIN_V; maxV = TURRET_MAX_V;

  enabled = true;
}

void Turret::setRange(byte min_h, byte max_h, byte min_v, byte max_v) {
  minH = min_h; maxH = max_h;
  minV = min_v; maxV = max_v;
}

void Turret::setDelta(byte dh, byte dv) {
  dH = dh;
  dV = dv;
}

void Turret::attach(byte laser) {
  pinMode(pinLaser = laser, OUTPUT);
  pinH = pinV = 0;
}

void Turret::attach(byte laser, byte p0, byte p1) {
  attach(laser);
  servoH.attach(pinH = p0);
  servoV.attach(pinV = p1);
}

void Turret::enable() {
  enabled = true;
}

void Turret::disable() {
  enabled = false;
}

byte Turret::getH() {
  return h;
}

byte Turret::getV() {
  return v;
}

void Turret::reset() {
  if (!servoH.attached() || !servoV.attached()) return;

  servoH.write(h = h0);
  servoV.write(v = v0);
}

void Turret::up() {
  if (enabled && servoV.attached() && v < maxV)
    servoV.write(v += dV);
}

void Turret::down() {
  if (enabled && servoV.attached() && v > minV)
    servoV.write(v -= dV);
}

void Turret::left() {
  if (enabled && servoH.attached() && h > minH)
    servoH.write(h -= dH);
}

void Turret::right() {
  if (enabled && servoH.attached() && h < maxH)
    servoH.write(h += dH);
}

void Turret::impulse(unsigned long length) {
  digitalWrite(pinLaser, HIGH);
  delay(length);
  digitalWrite(pinLaser, LOW);
}

void Turret::fire() {
  if (!enabled || !pinLaser) return;

  impulse(5);
}

/* vim: set ft=arduino et sw=2 ts=2: */

