/*
 * Impulses.cpp
 *
 */
#include <util/atomic.h>
#include <stdlib.h>

#include "Impulses.hpp"
#include "uassert.hpp"

namespace
{
// impulses callbacks
Impulses::IntCallback g_onLeftImpulse =NULL;
Impulses::IntCallback g_onRightImpulse=NULL;

// counter values
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

inline void interruptRoutine(volatile uint16_t &c, Impulses::IntCallback callback)
{
  // increment counter
  incCounter(c);
  // perform user's callback, if present
  if(callback!=NULL)
  {
    const uint16_t tmp=getCounter(c);
    callback(tmp);
  }
} // interruptRoutine()
} // unnamed namespace


// INT0 interrupt - left counter
ISR(INT0_vect)
{
  interruptRoutine(g_left, g_onLeftImpulse);
}

// INT1 interrupt - right counter
ISR(INT1_vect)
{
  interruptRoutine(g_right, g_onRightImpulse);
}


void Impulses::init(IntCallback onLeftImpulse, IntCallback onRightImpulse)
{
  // assign callbacks
  g_onLeftImpulse =onLeftImpulse;
  g_onRightImpulse=onRightImpulse;

  // initial values
  g_left =0;
  g_right=0;

  // set pin as input for INT0
  DDRD &=~_BV(2);
  PORTD|= _BV(2);
  // enable INT0
  MCUCR|=_BV(ISC01);
  MCUCR|=_BV(ISC00);
  GICR |=_BV(INT0);

  // set pin as input for INT1
  DDRD &=~_BV(3);
  PORTD|= _BV(3);
  // enable INT1
  MCUCR|=_BV(ISC11);
  MCUCR|=_BV(ISC10);
  GICR |=_BV(INT1);
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
