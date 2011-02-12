/*
 * EngSpeed.hpp
 *
 */
#ifndef INCLUDE_ENGSPEED_HPP_FILE
#define INCLUDE_ENGSPEED_HPP_FILE

#include "config.hpp"

#include <inttypes.h>

/** \brief control mechanism for the engines.
 */
struct EngSpeed
{
  struct Params
  {
    Params(char dir, uint8_t value):
      dir_(dir),
      value_(value)
    {
    }

    char    dir_;
    uint8_t value_;
  }; // struct Params

  static void init(void);

  static Params getLeftEngine(void);
  static void setLeftEngine(Params p);

  static Params getRightEngine(void);
  static void setRightEngine(Params);

  static void stop(void);
}; // struct EngSpeed

#endif
