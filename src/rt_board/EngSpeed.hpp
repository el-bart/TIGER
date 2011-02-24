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

  struct LeftEngine
  {
    static Params get(void)
    {
      return EngSpeed::getLeftEngine();
    }
    static void set(Params p)
    {
      EngSpeed::setLeftEngine(p);
    }
    static void modify(int8_t d)
    {
      EngSpeed::modifyLeft(d);
    }
  }; // struct LeftEngine

  struct RightEngine
  {
    static Params get(void)
    {
      return EngSpeed::getRightEngine();
    }
    static void set(Params p)
    {
      EngSpeed::setRightEngine(p);
    }
    static void modify(int8_t d)
    {
      EngSpeed::modifyRight(d);
    }
  }; // struct RightEngine


  static void init(void);

  static Params getLeftEngine(void);
  static void setLeftEngine(Params p);
  static void modifyLeft(int8_t d);

  static Params getRightEngine(void);
  static void setRightEngine(Params);
  static void modifyRight(int8_t d);

  static void stop(void);
}; // struct EngSpeed

#endif
