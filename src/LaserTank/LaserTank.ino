/*
 * Laser Tank
 */

#include <LaserTank.h>

const byte PIN_LED = 13;

const long BT_SPEED = 38400;
const long BT_TIMEOUT = 100;

LaserTank tank("СЛУ-01");

void setup() {
  tank.setDriverPins(2, 3, 4, 5);
  tank.setLaserPin(PIN_LED);
  tank.setHealthPins(A0, A1, A2, A3);
  tank.reset();

  Serial.begin(BT_SPEED);
  Serial.setTimeout(BT_TIMEOUT);
}

void loop() {
  byte cmd = 0;

  if (Serial.available() > 0) {
    cmd = Serial.read();
    switch (cmd) {
      // Tank movement
      case 'w': tank.forward();  break;
      case 'a': tank.left();     break;
      case 's': tank.backward(); break;
      case 'd': tank.right();    break;
      case 'x': tank.stop();     break;

      // Turret rotation
      case '8': tank.turretUp();    break;
      case '6': tank.turretRight(); break;
      case '4': tank.turretLeft();  break;
      case '2': tank.turretDown();  break;
      case '0': tank.turretStop();  break;

      // Fire laser gun
      case '5': tank.fire(); break;

      // Simulate hit
      case '*': tank.hit(); break;

      // Reset tank health
      case 'R': tank.reset(); break;

      // Send tank data
      case 'M': Serial.write(tank.getMaxHealth()); break;
      case 'H': Serial.write(tank.getHealth()); break;
      case 'N': Serial.print(tank.getName()); Serial.print('\n'); break;

      // Rename tank
      case '_': tank.setName(Serial.readStringUntil('\n')); break;
    }
    Serial.flush();
  }

  delay(10);
}

/* vim: set ft=arduino et sw=2 ts=2: */

