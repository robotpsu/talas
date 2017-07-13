#include "LaserTank.h"

const char LaserTank::stringTerminator = BT_STRING_TERMINATOR;
const byte LaserTank::maxHealth = TANK_MAX_HEALTH;

byte LaserTank::getMaxHealth() {
  return maxHealth;
}

LaserTank::LaserTank(String n) {
  pin_in1 = pin_in2 = pin_in3 = pin_in4 = pin_en1 = pin_en2
    = pin_laser = pin_dead = pin_life1 = pin_life2 = pin_life3 = 0;

  name = n;
}

void LaserTank::setDriverPins(byte in1, byte in2, byte in3, byte in4) {
  pinMode(pin_in1 = in1, OUTPUT);
  pinMode(pin_in2 = in2, OUTPUT);
  pinMode(pin_in3 = in3, OUTPUT);
  pinMode(pin_in4 = in4, OUTPUT);
}

void LaserTank::setSpeedPins(byte en1, byte en2) {
  pinMode(pin_en1 = en1, OUTPUT);
  pinMode(pin_en2 = en2, OUTPUT);
}

void LaserTank::setLaserPin(byte laser) {
  pinMode(pin_laser = laser, OUTPUT);
}

void LaserTank::setHealthPins(byte dead, byte life1, byte life2, byte life3) {
  pinMode(pin_dead  = dead,  OUTPUT);
  pinMode(pin_life1 = life1, OUTPUT);
  pinMode(pin_life2 = life2, OUTPUT);
  pinMode(pin_life3 = life3, OUTPUT);
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
  if (!health || !pin_in1) return;

  digitalWrite(pin_in1, HIGH);
  digitalWrite(pin_in2, LOW);
  digitalWrite(pin_in3, HIGH);
  digitalWrite(pin_in4, LOW);
}

void LaserTank::backward() {
  if (!health || !pin_in1) return;

  digitalWrite(pin_in1, LOW);
  digitalWrite(pin_in2, HIGH);
  digitalWrite(pin_in3, LOW);
  digitalWrite(pin_in4, HIGH);
}

void LaserTank::left() {
  if (!health || !pin_in1) return;

  digitalWrite(pin_in1, HIGH);
  digitalWrite(pin_in2, LOW);
  digitalWrite(pin_in3, LOW);
  digitalWrite(pin_in4, HIGH);
}

void LaserTank::right() {
  if (!health || !pin_in1) return;

  digitalWrite(pin_in1, LOW);
  digitalWrite(pin_in2, HIGH);
  digitalWrite(pin_in3, HIGH);
  digitalWrite(pin_in4, LOW);
}

void LaserTank::stop() {
  if (!pin_in1) return;

  digitalWrite(pin_in1, LOW);
  digitalWrite(pin_in2, LOW);
  digitalWrite(pin_in3, LOW);
  digitalWrite(pin_in4, LOW);
}

void LaserTank::turretUp() {
  if (!health) return;

  // TODO
}

void LaserTank::turretDown() {
  if (!health) return;

  // TODO
}

void LaserTank::turretLeft() {
  if (!health) return;

  // TODO
}

void LaserTank::turretRight() {
  if (!health) return;

  // TODO
}

void LaserTank::turretStop() {
  // TODO
}

void LaserTank::fire() {
  if (!health || !pin_laser) return;

  digitalWrite(pin_laser, HIGH);
  delay(5);
  digitalWrite(pin_laser, LOW);
}

void LaserTank::hit() {
  if (health) health--;
  showHealth();
}

void LaserTank::showHealth() {
  if (!pin_dead) return;

  digitalWrite(pin_life3, health >= 3 ? HIGH : LOW);
  digitalWrite(pin_life2, health >= 2 ? HIGH : LOW);
  digitalWrite(pin_life1, health >= 1 ? HIGH : LOW);
  digitalWrite(pin_dead, health ? LOW : HIGH);
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

