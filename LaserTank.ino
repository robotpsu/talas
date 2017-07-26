/*
 * Laser Tank
 */

#include "LaserTank.h"
#define PIN_LED 13
#define SERIAL_SPEED 9600

const char tankName[] = "СЛУ-01";

// Arduino pins
const byte
  pinIn1 = 2,
  pinIn2 = 3,
  pinIn3 = 4,
  pinIn4 = 7,
  pinEn1 = 5,
  pinEn2 = 6,
  pinLaser = 8,
  pinH = 9,
  pinV = 10;
#ifdef USE_SW_SERIAL
const byte
  pinRx = 11,
  pinTx = 12;
#endif

// HC-05 baud rates
const unsigned long btRates[] = {
  4800, 9600, 19200, 38400, 57600, 115200,
  23400, 460800, 921600, 1382400};
const size_t numRates = sizeof(btRates) / sizeof(unsigned long);

// HC-05 AT-commands to get some info
const char *atCmds[] = {
  "NAME", "VERSION", "ADDR", "PSWD", "ROLE", "CMODE", "UART"};
const size_t numCmds = sizeof(atCmds) / sizeof(char*);

const size_t bufSize = 64;
char buf[bufSize];

// Tank
#ifdef USE_SW_SERIAL
LaserTank tank(tankName, pinRx, pinTx);
#else
LaserTank tank(tankName);
#endif

void setup() {
  Serial.begin(SERIAL_SPEED);

  Serial.begin(BT_SPEED);
  Serial.setTimeout(BT_TIMEOUT);

  tank.attachDriver(pinIn1, pinIn2, pinIn3, pinIn4, pinEn1, pinEn2);
  tank.attachTurret(pinLaser);
  tank.attachHealth(A0, A1, A2, A3);

  tank.resetHealth();
  tank.resetTurret();
}

void loop() {
  tank.dispatch();
}

unsigned long getRate(SoftwareSerial &bt) {
  size_t r;
  for (size_t i = 0; i < numRates; i++) {
    bt.begin(btRates[i]);
    bt.setTimeout(100);
    bt.flush();
    bt.print("AT\r\n");
    r = bt.readBytes(buf, bufSize);
    if (r > 0) return btRates[i];
    bt.end();
  }
  return 0;
}

/*
  if (bt.isListening()) {
    for (size_t i = 0; i < numCmds; i++) {
      bt.write("AT+"); bt.write(cmds[i]); bt.write("?\r\n");
      s = bt.readString();
      if (!s.length()) continue;
      if (s.endsWith("OK\r\n")) {
        s.remove(s.indexOf('\r'));
        Serial.println(s);
      } else {
        Serial.write('+');
        Serial.print(cmds[i]);
        Serial.print(":FAIL");
      }
    }
    Serial.println("\nEnter AT-commands:");
  } else {
    Serial.println("Cannot connect to HC-05!");
    Serial.println("Is it in command mode?");
  }
  bt.begin(38400);
*/

/* vim: set ft=arduino et sw=2 ts=2: */

