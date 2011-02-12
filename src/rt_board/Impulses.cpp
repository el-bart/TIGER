/*
 * Impulses.cpp
 *
 */
#include <util/atomic.h>

#include "Impulses.hpp"

namespace
{
volatile uint16_t g_left;
volatile uint16_t g_right;

inline uint16_t getCounter(volatile const uint16_t &c)
{
  uint16_t tmp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    tmp=c;
  }
  return tmp;
} // getCounter()

inline uint16_t resetCounter(volatile uint16_t &c)
{
  uint16_t tmp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    tmp=c;
    c  =0;
  }
  return tmp;
} // resetCounter()

inline void incCounter(volatile uint16_t &c)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    ++c;
  }
} // incCounter()
} // unnamed namespace

// INT0 interrupt
ISR(INT0_vect)
{
  incCounter(g_left);
}

// INT1 interrupt
ISR(INT1_vect)
{
  incCounter(g_right);
}


void Impulses::init(void)
{
  // initial values
  g_left =0;
  g_right=0;

  // set pin as input for INT0
  DDRD &=~_BV(2);
  PORTD|= _BV(2);
  // enable INT0
  GICR |=INT0;
  MCUCR|=ISC01;
  MCUCR|=ISC00;

  // set pin as input for INT1
  DDRD &=~_BV(3);
  PORTD|= _BV(3);
  // enable INT1
  GICR |=INT1;
  MCUCR|=ISC11;
  MCUCR|=ISC10;
}

uint16_t Impulses::getLeftEngine(void)
{
  return getCounter(g_left);
}

uint16_t Impulses::getRightEngine(void)
{
  return getCounter(g_right);
}

uint16_t Impulses::resetLeftEngine(void)
{
  return resetCounter(g_left);
}

uint16_t Impulses::resetRightEngine(void)
{
  return resetCounter(g_right);
}
