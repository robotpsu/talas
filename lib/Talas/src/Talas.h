/**
 * \file
 * \brief   Talas library main header file.
 * \author  Vasiliy Polyakov <vp@psu.ru>
 * \author  RobotPSU https://robot.psu.ru/
 * \date    2017-2018
 */

#ifndef Talas_h
#define Talas_h

#include <Arduino.h>
#include "Turret.h"

#ifdef DOXYGEN
# define USE_AT_MODE  ///< Whether to build AT-commands mode support (boolean).
#else
//# define USE_AT_MODE true
#endif

#define TALAS_MAX_NAME_LENGTH  24  ///< Maximum name length (characters).
#define TALAS_MAX_HEALTH        3  ///< Maximum health (1..3).
#define TALAS_START_SPEED     150  ///< Start speed value (0..255).
#define TALAS_LIGHT_SAMPLES   100  ///< Number of light samples for illuminance check.
#define TALAS_LIGHT_DELAY      20  ///< Delay between light samples (ms).
#define TALAS_TIMER_FREQ     1000  ///< Hit detector timer frequency (Hz). 
#define TALAS_DISPATCH_DELAY   10  ///< Delay between dispatch iterations (ms).
#define IMPULSE_VARIANCE       50  ///< Impulse length variance (s).

#define SERIAL_SPEED  9600  ///< Serial port speed rate (baud).
#define BT_SPEED     38400  ///< Bluetooth speed rate (baud).
#define BT_TIMEOUT     100  ///< Bluetooth timeout (ms).
#define BT_STR_TERM '\n'    ///< Bluetooth string terminator.

/**
 * Impulse data.
 * Contains information about received laser impulses.
 */
typedef struct {
  uint16_t n;       ///< Impulse number.
  uint16_t length;  ///< Impulse length (ms).
  uint16_t value;   ///< Impulse value (\c HIGH or \c LOW).
} Impulse;

/**
 * Main TALAS class.
 * Laser tank (or basin) controlling code.
 * \nosubgrouping
 */
class Talas
{
public:
  /**
   * Constructor.
   * \param  n  TALAS name.
   */
  Talas(String n);

  /**
   * \name Device Attachments
   * For use in \c setup() function.
   * \{
   */
  /**
   * Attach health indicator.
   * Indicator uses digital pins for 4 LEDs.
   * \param  pin0  pin for red LED 'dead'.
   * \param  pin1  pin for green LED '1 life left'.
   * \param  pin2  pin for green LED '2 lives left'.
   * \param  pin3  pin for green LED '3 lives left'.
   */
  void attachHealth(uint8_t pin0, uint8_t pin1, uint8_t pin2, uint8_t pin3);
  /**
   * Attach DC motors driver with constant full speed.
   * \note Driver pins ENA and ENB must be closed by jumpers to use this method.
   * \param  pinIn1,pinIn2,pinIn3,pinIn4  Arduino pins for driver input.
   */
  void attachDriver(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinIn3, uint8_t pinIn4);
  /**
   * Attach DC motors driver with speed control.
   * \note There must be no jumpers on driver pins ENA and ENB to use this method.
   * \param  pinIn1,pinIn2,pinIn3,pinIn4  Arduino pins for driver input.
   * \param  pinEnA,pinEnB  Arduino pins for driver "enable" pins. Must be PWM-enabled.
   */
  void attachDriver(uint8_t pinIn1, uint8_t pinIn2, uint8_t pinIn3, uint8_t pinIn4,
                    uint8_t pinEnA, uint8_t pinEnB);
  /**
   * Attach turret with laser gun only.
   * \param  pinLaser  pin for laser gun.
   */
  void attachTurret(uint8_t pinLaser);
  /**
   * Attach movable turret with laser gun.
   * \param  pinLaser  pin for laser gun.
   * \param  pinH      pin for horizontal servo.
   * \param  pinV      pin for vertical servo.
   */
  void attachTurret(uint8_t pinLaser, uint8_t pinH, uint8_t pinV);
  /**
   * Attach hit detector.
   * \param  pinHit  Arduino pin for hit detector. Must be analog.
   */
  void attachHitDetector(uint8_t pinHit);
  /**
   * Attach Bluetooth communication module.
   * \note TALAS uses Serial1 for Bluetooth on Arduino Mega.
   */
  void attachBluetooth();
  /** \} */

  /**
   * \name Name
   * \{
   */
  /**
   * Set name.
   * \param  n  new name.
   */
  void name(String n);
  /**
   * Get name.
   * \return Name.
   */
  String name();
  /** \} */

  /**
   * \name Health Indicator
   * \{
   */
  /**
   * Get health.
   * \return Health.
   */
  uint8_t health();
  /**
   * Reset health (set health to a maximum value).
   */
  void resetHealth();
  /** \} */

  /**
   * \name Hit Detector
   * \{
   */
  /**
   * Handle timer interrupt and detect laser impulses.
   * Usage in Arduino sketch:
  \code
  ISR(TIMER3_COMPB_vect) {
    talas.handleImpulse();
  }
  \endcode
   */
  void handleImpulse();
  /**
   * Handle hits.
   * Decrease health and check if dead already.
   */
  void hit();
  /** \} */
  
  /**
   * \name Movement
   * \{
   */
  /**
   * Move forward.
   */
  void forward();
  /**
   * Move backward.
   */
  void backward();
  /**
   * Turn left.
   */
  void left();
  /**
   * Turn right.
   */
  void right();
  /**
   * Stop.
   */
  void stop();
  /**
   * Set speed.
   * \param  s  speed value.
   */
  void speed(uint8_t s);
  /**
   * Get speed.
   * \return Speed.
   */
  uint8_t speed();
  /**
   * Move slower.
   * Decrease speed.
   */
  void slower();
  /**
   * Move faster.
   * Decrease. speed.
   */
  void faster();
  /** \} */

  /**
   * \name Turret
   * See Turret class reference.
   * \{
   */
  void setTurretRange(uint8_t, uint8_t, uint8_t, uint8_t);
  void setTurretDelta(uint8_t, uint8_t);
  void resetTurret();
  uint8_t getTurretH();  // Turret horizontal angle
  uint8_t getTurretV();  // Turret vertical angle
  void turretUp();
  void turretDown();
  void turretLeft();
  void turretRight();
  /** \} */

  /**
   * \name Laser Gun
   * \{
   */
  /**
   * Fire laser gun.
   */
  void fire();
  /** \} */

  /**
   * \name Bluetooth Commands
   * \{
   */
  /**
   * Dispatch Bluetooth commands.
   * Usage in Arduino sketch:
   * \code
   * void loop() {
   *   talas.dispatch();
   * }
   * \endcode
   */
  void dispatch();
  /** \} */

private:
  static const size_t _impulses[];
  static const size_t _impulseMax;

  /// L289N driver input pins
  uint8_t _pinRight1, _pinRight2, _pinLeft1, _pinLeft2;
  /// L289N driver enable pins
  uint8_t _pinRightEn, _pinLeftEn;
  /// Health indicator (LED) control pins
  uint8_t _pinHealth[TALAS_MAX_HEALTH + 1];
  /// Hit sensor pin
  uint8_t _pinHit;

  String _name;              ///< Name.
  volatile uint8_t _health;  ///< Health (0..3).

  unsigned _lux;              ///< Illuminance.
  volatile Impulse _impulse;  ///< Received impulse data.

  // Motors speed
  uint8_t _speedRight, _speedLeft;

  Turret _turret;

  void showHealth();
  void move(uint8_t);
  int getLight();
  void checkLight();
  void setImpulseTimer();

#if defined(SERIAL_PORT_HARDWARE_OPEN) && defined(USE_AT_MODE)
  // HC-05 Bluetooth module AT-commands mode
  static const unsigned long _rates[];  ///< List of supported speed rates.
  static const size_t _numRates;        ///< Number of supported speed rates.
  static const char *_cmds[];           ///< Info commands list.
  static const size_t _numCmds;         ///< Number of info commands.
  unsigned long _atMode;                ///< Actual speed rate (baud).

  unsigned long inCmdMode();  ///< Whether module is in AT-commands mode.
  void printBtInfo();         ///< Send information about module.
#endif
};

#endif

/* vim: set ft=arduino et sw=2 ts=2: */
