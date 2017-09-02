/**
 * \file
 * \brief    Talas class implementation.
 * \author   Vasiliy Polyakov <vp@psu.ru>
 * \authors  RobotPSU https://vk.com/robotpsu
 * \date     2017
 */

#include "Talas.h"

#ifdef DOXYGEN
# define BT  ///< Bluetooth serial object.
#else
# ifdef SERIAL_PORT_HARDWARE_OPEN
#  define BT SERIAL_PORT_HARDWARE_OPEN
# else
#  define BT SERIAL_PORT_HARDWARE
# endif
#endif

/**
 * List of laser gun impulses and pauses between them.
 * Contains lengths in ms.
 */
const size_t Talas::_impulses[] = {100, 80, 100};
const size_t Talas::_impulseMax = sizeof(Talas::_impulses) / sizeof(size_t);

Talas::Talas(String n) : _name(n) {
  _pinRight1 = _pinRight2 = _pinLeft1 = _pinLeft2
    = _pinRightEn = _pinLeftEn = _pinHealth[0] = 0;
  _health = TALAS_MAX_HEALTH;
  _speedRight = _speedLeft = TALAS_START_SPEED;
}

void Talas::attachHealth(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3) {
  pinMode(_pinHealth[0] = pin0, OUTPUT);
  pinMode(_pinHealth[1] = pin1, OUTPUT);
  pinMode(_pinHealth[2] = pin2, OUTPUT);
  pinMode(_pinHealth[3] = pin3, OUTPUT);
  resetHealth();
}

void Talas::attachDriver(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinIn3, uint8_t pinIn4) {
  pinMode(_pinRight1 = pinIn1, OUTPUT);
  pinMode(_pinRight2 = pinIn2, OUTPUT);
  pinMode(_pinLeft1  = pinIn3, OUTPUT);
  pinMode(_pinLeft2  = pinIn4, OUTPUT);
  _pinRightEn = _pinLeftEn = 0;
}

void Talas::attachDriver(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinIn3, uint8_t pinIn4,
                         uint8_t pinEnA, uint8_t pinEnB) {
  attachDriver(pinIn1, pinIn2, pinIn3, pinIn4);
  pinMode(_pinRightEn = pinEnA, OUTPUT);
  pinMode(_pinLeftEn  = pinEnB, OUTPUT);
}

void Talas::attachTurret(uint8_t pinLaser) {
  _turret.attach(pinLaser);
  _turret.reset();
}

void Talas::attachTurret(uint8_t pinLaser, uint8_t pinH, uint8_t pinV) {
  _turret.attach(pinLaser, pinH, pinV);
  _turret.reset();
}

void Talas::attachHitDetector(uint8_t pinHit) {
  _pinHit = pinHit;
  checkLight();       // Get illuminance and light "zero level"
  setImpulseTimer();  // Set up timer interrupt
}

void Talas::attachBluetooth() {
#if defined(SERIAL_PORT_HARDWARE_OPEN) && defined(USE_AT_MODE)
  // Try AT-commands mode.
  if (unsigned long rate = inCmdMode()) {
    SERIAL_PORT_MONITOR.begin(SERIAL_SPEED);
    SERIAL_PORT_MONITOR.print("TALAS «");
    SERIAL_PORT_MONITOR.print(_name);
    SERIAL_PORT_MONITOR.println("» configuration");
    SERIAL_PORT_MONITOR.println("\nBluetooth module info:");
    printBtInfo();
    SERIAL_PORT_MONITOR.println("\nEnter commands:");
    return;
  }
#endif
  BT.begin(BT_SPEED);
  BT.setTimeout(BT_TIMEOUT);
  BT.flush();
}

void Talas::name(String n) {
  _name = n.substring(0, TALAS_MAX_NAME_LENGTH);
}

String Talas::name() {
  return _name;
}

void Talas::showHealth() {
  if (!_pinHealth[0]) return;

  digitalWrite(_pinHealth[0], !_health);
  for (uint8_t i = 1; i <= TALAS_MAX_HEALTH; i++)
    digitalWrite(_pinHealth[i], _health >= i);
}

void Talas::resetHealth() {
  _health = TALAS_MAX_HEALTH;
  showHealth();
  _turret.enable();
}

uint8_t Talas::health() {
  return _health;
}

/**
 * Move or stop TALAS.
 * Send signals to the driver.
 * \param  direction  bit mask for motors:
 * - bits 0 and 1 for motor A;
 * - bits 2 and 3 for motor B.
 */
void Talas::move(uint8_t direction) {
  if (!_health || !_pinRight1 || !_pinLeft1) return;

  digitalWrite(_pinRight1, bitRead(direction, 0));
  digitalWrite(_pinRight2, bitRead(direction, 1));
  digitalWrite(_pinLeft1,  bitRead(direction, 2));
  digitalWrite(_pinLeft2,  bitRead(direction, 3));

  if (_pinRightEn)
    analogWrite(_pinRightEn, direction ? _speedRight : 0);
  if (_pinLeftEn)
    analogWrite(_pinLeftEn,  direction ? _speedLeft  : 0);
}

// If TALAS moves in wrong direction then reconnect driver
// and motors in the right order.
void Talas::forward() {
  move(0b0101);
}

void Talas::backward() {
  move(0b1010);
}

void Talas::left() {
  move(0b1001);
}

void Talas::right() {
  move(0b0110);
}

void Talas::stop() {
  move(0);
}

void Talas::speed(uint8_t s) {
  _speedRight = _speedLeft = s;
}

uint8_t Talas::speed() {
  return _speedRight;
}

void Talas::faster() {
  _speedRight++;
  _speedLeft++;
}

void Talas::slower() {
  _speedRight--;
  _speedLeft--;
}

void Talas::setTurretRange(uint8_t minH, uint8_t maxH, uint8_t minV, uint8_t maxV) {
  _turret.setRange(minH, maxH, minV, maxV);
}

void Talas::setTurretDelta(uint8_t dH, uint8_t dV) {
  _turret.setDelta(dH, dV);
}

uint8_t Talas::getTurretH() {
  return _turret.getH();
}

uint8_t Talas::getTurretV() {
  return _turret.getV();
}

void Talas::turretUp() {
  _turret.up();
}

void Talas::turretDown() {
  _turret.down();
}

void Talas::turretLeft() {
  _turret.left();
}

void Talas::turretRight() {
  _turret.right();
}

void Talas::resetTurret() {
  _turret.reset();
}

void Talas::fire() {
  for (size_t i = 0; i < _impulseMax; i++) {
    if (i & 1) {
      delay(_impulses[i]);
    } else {
      _turret.impulse(_impulses[i]);
    }
  }
}

/**
 * Read value from photoresistor if it's connected.
 */
int Talas::getLight() {
  return _pinHit ? analogRead(_pinHit) : 0;
}

/**
 * Get illuminance statistics and light conditions.
 */
void Talas::checkLight() {
  float m = 0., s = 0.;
  unsigned long l;

  for (int i = 0; i < TALAS_LIGHT_SAMPLES; i++) {
    l = getLight();
    m += l; s += l * l;
    delay(TALAS_LIGHT_DELAY);
  }
  m /= TALAS_LIGHT_SAMPLES;             // Mean
  s = s / TALAS_LIGHT_SAMPLES - m * m;  // Variance

  _lux = constrain(int(m + PI * s), 0, 1023);
}

void Talas::setImpulseTimer() {
  // See http://www.robotshop.com/letsmakerobots/arduino-101-timers-and-interrupts
  noInterrupts();                        // Disable interrupts
  TCCR3A = TCCR3B = TCNT3 = 0;           // Reset registers
  OCR3B = F_CPU / TALAS_TIMER_FREQ - 1;  // Set frequency
  bitSet(TCCR3B, WGM32);                 // CTC mode
  bitSet(TCCR3B, CS30);                  // Internal clock
  bitSet(TIMSK3, OCIE3B);                // Enable compare handler
  interrupts();                          // Enable interrupts

  _impulse.n = _impulse.length = 0;
  _impulse.value = LOW;
}

void Talas::handleImpulse() {
  TCNT3 = 0;  // Reset counter

  int value = getLight() > _lux;

  if (value != _impulse.value) {
    // Next impulse detected
    if ((_impulse.n & 1  // Some binary magic
        || _impulse.length > _impulses[_impulse.n] - IMPULSE_VARIANCE
        && _impulse.length < _impulses[_impulse.n] + IMPULSE_VARIANCE)
        && _impulse.n < _impulseMax) {
      _impulse.n++;  // OK, continue detection
    } else {
      _impulse.n = 0;  // False alarm
    }
    _impulse.length = 0;
  } else {
    _impulse.length++;  // Still detecting impulse
  }
  _impulse.value = value;

  if (_impulse.n >= _impulseMax) {
    _impulse.n = _impulse.length = 0;
    hit();  // Пулею пробито днище котелка…
  }
}

void Talas::hit() {
  if (!_health) return;

  if (--_health) {
    _turret.enable();
  } else {
    _turret.disable();
    stop();
  }
  showHealth();
}

void Talas::dispatch() {
#if defined(SERIAL_PORT_HARDWARE_OPEN) && defined(USE_AT_MODE)
  if (_atMode) {
    if (BT.available())
      SERIAL_PORT_MONITOR.write(BT.read());
    if (SERIAL_PORT_MONITOR.available()) {
      int b = SERIAL_PORT_MONITOR.read();
      SERIAL_PORT_MONITOR.write(b);
      BT.write(b);
    }
    return;
  }
#endif
  if (BT.available()) {
    switch (BT.read()) {
      // Move
      case 'w': forward();  break;
      case 'a': left();     break;
      case 's': backward(); break;
      case 'd': right();    break;
      case 'x': stop();     break;

      // Control speed
      case '-': slower(); break;
      case '+': faster(); break;
      case '^': speed(BT.read());  break;
      case 'V': BT.write(speed()); break;

      // Rotate turret
      case '8': _turret.up();    break;
      case '6': _turret.right(); break;
      case '4': _turret.left();  break;
      case '2': _turret.down();  break;
      case '0': _turret.reset(); break;
      case '/':  // Send turret position
        BT.write(_turret.getH());
        BT.write(_turret.getV());
        break;

      // Fire laser gun
      case '5': fire(); break;

      // Health
      case 'M': BT.write(TALAS_MAX_HEALTH); break;
      case 'H': BT.write(_health); break;
      case 'R': resetHealth(); break;
      case '*': hit(); break;  // Simulate hit

      // Name
      case 'N': BT.print(_name); BT.print(BT_STR_TERM); break;
      case 'n': name(BT.readStringUntil(BT_STR_TERM));  break;
    }
    BT.flush();
  }
  delay(TALAS_DISPATCH_DELAY);
}

/*
 * Bluetooth module AT-commands mode
 */
#if defined(SERIAL_PORT_HARDWARE_OPEN) && defined(USE_AT_MODE)
// HC-05 baud rates
const unsigned long Talas::_rates[] = {
  4800, 9600, 19200, 38400, 57600, 115200, 23400, 460800, 921600, 1382400};
const size_t Talas::_numRates = sizeof(Talas::_rates) / sizeof(unsigned long);

// HC-05 AT-commands to get some info
const char *Talas::_cmds[] = {
  "NAME", "VERSION", "UART", "ADDR", "PSWD", "ROLE", "CMODE", "CLASS", "IAC"};
const size_t Talas::_numCmds = sizeof(Talas::_cmds) / sizeof(char*);

unsigned long Talas::inCmdMode() {
  char buf[32];

  for(size_t i = 0; i < _numRates; i++) {
    BT.begin(_rates[i]);
    BT.setTimeout(100);
    BT.flush();
    BT.write("AT\r\n");
    if (BT.readBytes(buf, 32) > 0)
      return _atMode = _rates[i];
    BT.end();
  }

  return _atMode = 0;
}

void Talas::printBtInfo() {
  for (size_t i = 0; i < _numCmds; i++) {
    BT.write("AT+"); BT.write(_cmds[i]); BT.write("?\r\n");
    String s = BT.readString();
    if (!s.length()) continue;
    if (s.endsWith("OK\r\n")) {
      s.remove(s.indexOf('\r'));
      SERIAL_PORT_MONITOR.println(s);
    } else {
      SERIAL_PORT_MONITOR.write('+');
      SERIAL_PORT_MONITOR.print(_cmds[i]);
      SERIAL_PORT_MONITOR.print(":FAIL");
    }
  }
}
#endif

/* vim: ft=arduino et sw=2 ts=2: */
