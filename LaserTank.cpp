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
  speed1 = speed2 = TANK_START_SPEED;
}

void LaserTank::attachBluetooth() {
  bt = (SoftwareSerial*)&Serial;
  bt->begin(TANK_BT_SPEED);
  bt->setTimeout(TANK_BT_TIMEOUT);
}

void LaserTank::attachBluetooth(byte rx, byte tx) {
  SoftwareSerial b(rx, tx);
  bt = &b;
  bt->begin(TANK_BT_SPEED);
  bt->setTimeout(TANK_BT_TIMEOUT);
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

void LaserTank::move(byte direction) {
  if (!health || !pinIn1) return;

  digitalWrite(pinIn1, bitRead(direction, 0));
  digitalWrite(pinIn2, bitRead(direction, 1));
  digitalWrite(pinIn3, bitRead(direction, 2));
  digitalWrite(pinIn4, bitRead(direction, 3));

  if (pinEn1)
    analogWrite(pinEn1, direction ? speed1 : 0);
  if (pinEn2)
    analogWrite(pinEn2, direction ? speed2 : 0);
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
  speed1++;
  speed2++;
}

void LaserTank::slower() {
  speed1--;
  speed2--;
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

void LaserTank::dispatch() {
  if (!bt) return;

  byte n = bt->available();
  byte cmd = 0;

  if (n > 0) {
    cmd = bt->read();
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
      case '8': turret.up();    break;
      case '6': turret.right(); break;
      case '4': turret.left();  break;
      case '2': turret.down();  break;

      // Move turret to the initial position
      case '0': turret.reset(); break;

      // Fire laser gun
      case '5': turret.fire(); break;

      // Simulate hit
      case '*': hit(); break;

      // Reset tank health
      case 'r': resetHealth(); break;

      // Send tank data
      case 'M': bt->write(maxHealth); break;
      case 'H': bt->write(health); break;
      case 'N': bt->print(name); bt->print(stringTerminator); break;
      case '/': bt->write(turret.getH()); bt->write(turret.getV()); break;

      // Rename tank
      case 'n': setName(bt->readStringUntil(stringTerminator)); break;
    }
    bt->flush();
  }
  delay(TANK_DISPATCH_DELAY);
}

/* vim: set ft=arduino et sw=2 ts=2: */

