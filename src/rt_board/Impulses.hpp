/*
 * Impulses.hpp
 *
 */
#ifndef INCLUDE_IMPULSES_HPP_FILE
#define INCLUDE_IMPULSES_HPP_FILE

#include "config.hpp"

#include <inttypes.h>

/** \brief impulse counting mechanism.
 */
struct Impulses
{
  typedef void (*IntCallback)(uint16_t cnt);

  static void init(IntCallback onLeftImpulse, IntCallback onRightImpulse);

  static uint16_t getLeftEngine(void);
  static uint16_t getRightEngine(void);

  static uint16_t resetLeftEngine(void);
  static uint16_t resetRightEngine(void);
}; // struct Impulses

#endif
