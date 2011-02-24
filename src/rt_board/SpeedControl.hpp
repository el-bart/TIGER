/*
 * SpeedControl.hpp
 *
 */
#ifndef INCLUDE_SPEEDCONTROL_HPP_FILE
#define INCLUDE_SPEEDCONTROL_HPP_FILE

#include "config.hpp"
#include "EngSpeed.hpp"

#include <inttypes.h>


void onLeftImpulseControl(uint16_t c);
void onRightImpulseControl(uint16_t c);

/** \brief feedback for controling PWM of the engines to achieve required speed.
 */
struct SpeedControl
{
  struct Params
  {
    // TODO: this config is invalid - for PWM one needs % fill, for
    //       speed control timespan between impulses. it has to be reogranized.
    Params(uint16_t count, const EngSpeed::Params &speed):
      count_(count),
      speed_(speed)
    {
    }

    uint16_t         count_;    ///< number of steps to perform
    EngSpeed::Params speed_;    ///< speed to ride with
  }; // struct Params

  static void init(void);

  static void leftEngine(Params p);
  static void rightEngine(Params p);

  static void stop(void);
}; // struct SpeedControl

#endif
