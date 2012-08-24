/*
 * SpeedControl.hpp
 *
 */
#ifndef INCLUDE_SPEEDCONTROL_HPP_FILE
#define INCLUDE_SPEEDCONTROL_HPP_FILE

#include "config.hpp"

#include <inttypes.h>


void onLeftImpulseControl(uint16_t c);
void onRightImpulseControl(uint16_t c);

/** \brief feedback for controling PWM of the engines to achieve required speed.
 */
struct SpeedControl
{
  struct Params
  {
    Params(uint16_t count, int8_t dir, uint8_t delay):
      count_(count),
      dir_(dir),
      delay_(delay)
    {
    }

    uint16_t count_;        ///< number of steps to perform
    int8_t   dir_;          ///< -/0/+ == left/stop/right
    uint8_t  delay_;        ///< expected delay between impulses, measured in multiple of 1[ms] (1.024[ms], to be precise)
  }; // struct Params

  static void init(void);

  static void leftEngine(Params p);
  static void rightEngine(Params p);

  static void stop(void);
}; // struct SpeedControl

#endif
