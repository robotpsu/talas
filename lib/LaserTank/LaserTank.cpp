/*
 * Laser tank class implementation
 */

#include "LaserTank.h"

const char LaserTank::stringTerminator = BT_STRING_TERMINATOR;
const byte LaserTank::maxHealth = TANK_MAX_HEALTH;

byte LaserTank::getMaxHealth() {
  return maxHealth;
}

LaserTank::LaserTank(String n) : name(n) {
  pinIn1 = pinIn2 = pinIn3 = pinIn4 = pinEn1 = pinEn2
    = pinDead = pinLife1 = pinLife2 = pinLife3 = 0;
}

void LaserTank::attachDriver(byte in1, byte in2, byte in3, byte in4) {
  pinMode(pinIn1 = in1, OUTPUT);
  pinMode(pinIn2 = in2, OUTPUT);
  pinMode(pinIn3 = in3, OUTPUT);
  pinMode(pinIn4 = in4, OUTPUT);
  pinEn1 = pinEn2 = 0;
}

void LaserTank::attachDriver(byte in1, byte in2, byte in3, byte in4, byte en1, byte en2) {
  attachDriver(in1, in2, in3, in4);
  pinMode(pinEn1 = en1, OUTPUT);
  pinMode(pinEn2 = en2, OUTPUT);
}

void LaserTank::attachTurret(byte laser) {
  turret.attach(laser);
}

void LaserTank::attachTurret(byte laser, byte p0, byte p1) {
  turret.attach(laser, p0, p1);
}

void LaserTank::attachHealth(byte dead, byte life1, byte life2, byte life3) {
  pinMode(pinDead  = dead,  OUTPUT);
  pinMode(pinLife1 = life1, OUTPUT);
  pinMode(pinLife2 = life2, OUTPUT);
  pinMode(pinLife3 = life3, OUTPUT);
}

byte LaserTank::getHealth() {
  return health;
}

String LaserTank::getName() {
  return name;
}

void LaserTank::setName(String n) {
  name = n.substring(0, TANK_MAX_NAME_LENGTH);
}

void LaserTank::setTurretRange(byte h0, byte h1, byte v0, byte v1) {
  turret.setRange(h0, h1, v0 ,v1);
}

void LaserTank::setTurretDelta(byte dh, byte dv) {
  turret.setDelta(dh, dv);
}

void LaserTank::forward() {
  if (!health || !pinIn1) return;

  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, LOW);
  digitalWrite(pinIn3, HIGH);
  digitalWrite(pinIn4, LOW);
}

void LaserTank::backward() {
  if (!health || !pinIn1) return;

  digitalWrite(pinIn1, LOW);
  digitalWrite(pinIn2, HIGH);
  digitalWrite(pinIn3, LOW);
  digitalWrite(pinIn4, HIGH);
}

void LaserTank::left() {
  if (!health || !pinIn1) return;

  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, LOW);
  digitalWrite(pinIn3, LOW);
  digitalWrite(pinIn4, HIGH);
}

void LaserTank::right() {
  if (!health || !pinIn1) return;

  digitalWrite(pinIn1, LOW);
  digitalWrite(pinIn2, HIGH);
  digitalWrite(pinIn3, HIGH);
  digitalWrite(pinIn4, LOW);
}

void LaserTank::stop() {
  if (!pinIn1) return;

  digitalWrite(pinIn1, LOW);
  digitalWrite(pinIn2, LOW);
  digitalWrite(pinIn3, LOW);
  digitalWrite(pinIn4, LOW);
}

void LaserTank::hit() {
  if (!health) return;

  if (--health) {
    turret.enable();
  } else {
    turret.disable();
  }
  showHealth();
}

void LaserTank::showHealth() {
  if (!pinDead) return;

  digitalWrite(pinLife3, health >= 3 ? HIGH : LOW);
  digitalWrite(pinLife2, health >= 2 ? HIGH : LOW);
  digitalWrite(pinLife1, health >= 1 ? HIGH : LOW);
  digitalWrite(pinDead, health ? LOW : HIGH);
}

void LaserTank::resetHealth() {
  health = maxHealth;
  turret.enable();
}

void LaserTank::resetTurret() {
  turret.reset();
}

void LaserTank::dispatch(byte cmd) {
    switch (cmd) {
      // Move tank
      case 'w': forward();  break;
      case 'a': left();     break;
      case 's': backward(); break;
      case 'd': right();    break;
      case 'x': stop();     break;

      // Rotate turret
      case '8': turret.up();    break;
      case '6': turret.right(); break;
      case '4': turret.left();  break;
      case '2': turret.down();  break;
      case '0': turret.reset(); break;

      // Fire laser gun
      case '5': turret.fire(); break;

      // Simulate hit
      case '*': hit(); break;

      // Reset tank health
      case 'r': resetHealth(); break;

      // Send tank data
      case 'M': Serial.write(maxHealth); break;
      case 'H': Serial.write(health); break;
      case 'N': Serial.print(name); Serial.print(stringTerminator); break;
      case '/': Serial.write(turret.getH()); Serial.write(turret.getV()); break;

      // Rename tank
      case 'n': setName(Serial.readStringUntil(stringTerminator)); break;
    }
    Serial.flush();
}

/* vim: set ft=arduino et sw=2 ts=2: */

