/**
 * \file
 * \brief   Turret class header file.
 * \author  Vasiliy Polyakov <vp@psu.ru>
 * \author  RobotPSU https://robot.psu.ru/
 * \date    2017-2018
 */

#ifndef Turret_h
#define Turret_h

#include <Arduino.h>
#include <Servo.h>

// Turret angles
#define TURRET_START_H 90  ///< Starting horizontal angle (degrees)
#define TURRET_START_V 90  ///< Starting horizontal angle (degrees)
#define TURRET_DELTA_H  1  ///< Horizontal increment/decrement (degrees)
#define TURRET_DELTA_V  1  ///< Horizontal increment/decrement (degrees)
#define TURRET_MIN_H   30  ///< Minimum horizontal angle (degrees)
#define TURRET_MAX_H  150  ///< Maximum horizontal angle (degrees)
#define TURRET_MIN_V   70  ///< Minimum vertical angle (degrees)
#define TURRET_MAX_V  150  ///< Maximum vertical angle (degrees)

/**
 * TALAS rotating turret with laser gun.
 */
class Turret
{
public:
  uint8_t h0;  ///< Initial horizontal angle.
  uint8_t v0;  ///< Initial vertical angle.

  /**
   * Constructor.
   */
  Turret();

  /**
   * Attach turret with laser gun only.
   * \param  pinLaser  pin for laser gun.
   */
  void attach(uint8_t pinLaser);
  /**
   * Attach movable turret with laser gun.
   * \param  pinLaser  pin for laser gun.
   * \param  pinH      pin for horizontal servo.
   * \param  pinV      pin for vertical servo.
   */
  void attach(uint8_t pinLaser, uint8_t pinH, uint8_t pinV);

  /**
   * Set servos angle range.
   * \param  minH,maxH  horizontal range.
   * \param  minV,maxV  vertical range.
   */
  void setRange(uint8_t minH, uint8_t maxH, uint8_t minV, uint8_t maxV);
  /**
   * Set servos angle increment/decrement value.
   * \param  dH,dV  new increment/decrement values.
   */
  void setDelta(uint8_t dH, uint8_t dV);

  void enable();   ///< Enable turret.
  void disable();  ///< Disable turret.

  /**
   * Reset turret.
   * Turn to the initial position.
   */
  void reset();

  /**
   * Get current horizontal position.
   * \return  Horizontal angle (degrees).
   */
  uint8_t getH();
  /**
   * Get current vertical position.
   * \return  Vertical angle (degrees).
   */
  uint8_t getV();

  void up();     ///< Rotate turret up.
  void down();   ///< Rotate turret down.
  void left();   ///< Rotate turret left.
  void right();  ///< Rotate turret right.

  /**
   * Fire laser gun.
   * Emit a laser impulse with specified length in ms.
   * \param  length  impulse length (ms).
   */
  void impulse(unsigned long length);

private:
  volatile bool _enabled;  // If turret enabled.
  uint8_t _pinLaser;       // Pin for laser gun.
  Servo _servoH, _servoV;  // Servo objects.
  uint8_t _h, _v;          // Turret angles (values for servos).
  uint8_t _dH, _dV;        // Angle increment/decrement values.
  uint8_t _minH, _maxH;    // Horizontal range.
  uint8_t _minV, _maxV;    // Vertical range.
};

#endif

/* vim: set ft=arduino et sw=2 ts=2: */
