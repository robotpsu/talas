#include "LaserTank.h"

const char LaserTank::stringTerminator = BT_STRING_TERMINATOR;

const byte LaserTank::maxHealth = TANK_MAX_HEALTH;

const byte LaserTank::minAngleH = TURRET_MIN_H;
const byte LaserTank::maxAngleH = TURRET_MAX_H;
const byte LaserTank::minAngleV = TURRET_MIN_V;
const byte LaserTank::maxAngleV = TURRET_MAX_V;
const byte LaserTank::deltaH = TURRET_DELTA_H;
const byte LaserTank::deltaV = TURRET_DELTA_V;

byte LaserTank::getMaxHealth() {
  return maxHealth;
}

LaserTank::LaserTank(String n) : name(n) {
  pinIn1 = pinIn2 = pinIn3 = pinIn4 = pinEn1 = pinEn2
    = pinTurretH = pinTurretV = pinLaser
    = pinDead = pinLife1 = pinLife2 = pinLife3 = 0;

  angleH = TURRET_START_H;
  angleV = TURRET_START_V;
}

void LaserTank::setDriverPins(byte in1, byte in2, byte in3, byte in4) {
  pinMode(pinIn1 = in1, OUTPUT);
  pinMode(pinIn2 = in2, OUTPUT);
  pinMode(pinIn3 = in3, OUTPUT);
  pinMode(pinIn4 = in4, OUTPUT);
}

void LaserTank::setSpeedPins(byte en1, byte en2) {
  pinMode(pinEn1 = en1, OUTPUT);
  pinMode(pinEn2 = en2, OUTPUT);
}

void LaserTank::setTurretPins(byte h, byte v) {
  turretH.attach(pinTurretH = h);
  turretV.attach(pinTurretV = v);
}

void LaserTank::setLaserPin(byte laser) {
  pinMode(pinLaser = laser, OUTPUT);
}

void LaserTank::setHealthPins(byte dead, byte life1, byte life2, byte life3) {
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

void LaserTank::turretUp() {
  if (health && pinTurretV && angleV < maxAngleV)
    turretV.write(angleV += deltaV);
}

void LaserTank::turretDown() {
  if (health && pinTurretV && angleV > minAngleV)
    turretV.write(angleV -= deltaV);
}

void LaserTank::turretLeft() {
  if (health && pinTurretH && angleH > minAngleH)
    turretH.write(angleH -= deltaH);
}

void LaserTank::turretRight() {
  if (health && pinTurretH && angleH < maxAngleH)
    turretH.write(angleH += deltaH);
}

void LaserTank::turretStop() {}

void LaserTank::fire() {
  if (!health || !pinLaser) return;

  digitalWrite(pinLaser, HIGH);
  delay(5);
  digitalWrite(pinLaser, LOW);
}

void LaserTank::hit() {
  if (health) health--;
  showHealth();
}

void LaserTank::showHealth() {
  if (!pinDead) return;

  digitalWrite(pinLife3, health >= 3 ? HIGH : LOW);
  digitalWrite(pinLife2, health >= 2 ? HIGH : LOW);
  digitalWrite(pinLife1, health >= 1 ? HIGH : LOW);
  digitalWrite(pinDead, health ? LOW : HIGH);
}

void LaserTank::reset() {
  health = maxHealth;
}

void LaserTank::dispatch(byte cmd) {
    switch (cmd) {
      // Tank movement
      case 'w': forward();  break;
      case 'a': left();     break;
      case 's': backward(); break;
      case 'd': right();    break;
      case 'x': stop();     break;

      // Turret rotation
      case '8': turretUp();    break;
      case '6': turretRight(); break;
      case '4': turretLeft();  break;
      case '2': turretDown();  break;
      case '0': turretStop();  break;

      // Fire laser gun
      case '5': fire(); break;

      // Simulate hit
      case '*': hit(); break;

      // Reset tank health
      case 'R': reset(); break;

      // Send tank data
      case 'M': Serial.write(getMaxHealth()); break;
      case 'H': Serial.write(getHealth()); break;
      case 'N': Serial.print(getName()); Serial.print(stringTerminator); break;

      // Rename tank
      case '_': setName(Serial.readStringUntil(stringTerminator)); break;
    }
    Serial.flush();
}

/* vim: set ft=arduino et sw=2 ts=2: */

