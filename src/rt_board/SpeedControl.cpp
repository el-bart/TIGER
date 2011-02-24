/*
 * SpeedControl.cpp
 *
 */
#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "SpeedControl.hpp"
#include "uassert.hpp"

namespace
{
SpeedControl::Params g_left( 0, EngSpeed::Params(0,0) );
uint16_t             g_leftToGo=0;

SpeedControl::Params g_right(0, EngSpeed::Params(0,0) );
uint16_t             g_rightToGo=0;

// global cycles count
uint8_t g_timerCycles=0;

class Timestamp
{
public:
  Timestamp(void):
    ts_( (g_timerCycles<<8) | TCNT0 )
  {
  }

  uint16_t get(void) const
  {
    return ts_;
  }

  uint16_t operator-(const Timestamp other) const
  {
    if( get()>=other.get() )
      return get()-other.get();
    return get() + (0xFFFF-other.get());
  }

private:
  uint16_t ts_;
}; // class Timestamp

// when last impulse had arrived
Timestamp g_lastLeftImpulse;
Timestamp g_lastRightImpulse;

template<typename TEng>
inline void impulseHandle(Timestamp &lastImpulse, uint16_t &impulsesToGo, const SpeedControl::Params &cfg)
{
  const Timestamp now;      // save this time ASAP

  if(impulsesToGo==0)
  {
    TEng::stop();
    // TODO: callback for the next orders?
    return;
  }
  --impulsesToGo;

  const uint16_t diff   =now-lastImpulse;
  const uint8_t  expDiff=0xFF-cfg.speed_.value_;

  // is it perfect?
  if(expDiff==diff)
    return;

  // TODO: think of something more sophisticated
  // simple +/- control
  if(expDiff>diff)
    TEng::modify(+10);
  else
    TEng::modify(-10);
} // impulseHandle()


template<typename TEng>
inline void checkNoImpulse(const Timestamp &now, const Timestamp &lastImpulse, const uint16_t impulsesLeft)
{
  // if no impulses are left, do nothing
  if(impulsesLeft==0)
    return;
  // no impulse for some time?
  if(now-lastImpulse>0xFF)
    TEng::modify(+40);      // give extra power!
} // checkNoImpulse
} // unnamed namespace


// timer 0 interrupt
ISR(TIMER0_OVF_vect)
{
  ++g_timerCycles;
  const Timestamp now;
  checkNoImpulse<EngSpeed::LeftEngine>(now, g_lastLeftImpulse, g_leftToGo);
  checkNoImpulse<EngSpeed::RightEngine>(now, g_lastRightImpulse, g_rightToGo);
}


void onLeftImpulseControl(uint16_t c)
{
  impulseHandle<EngSpeed::LeftEngine>(g_lastLeftImpulse, g_leftToGo, g_left);
}

void onRightImpulseControl(uint16_t c)
{
  impulseHandle<EngSpeed::RightEngine>(g_lastRightImpulse, g_rightToGo, g_right);
}


void SpeedControl::init(void)
{
  // assign initial values
  g_leftToGo   =0;
  g_rightToGo  =0;
  g_timerCycles=0;

  // set timer 0
  TCNT0 =0;             // start counting from 0
  TCCR0&=~_BV(CS02);    // set prescaller to clk/64
  TCCR0|= _BV(CS01);    // ...
  TCCR0|= _BV(CS00);    // ...
  TIMSK|= _BV(TOIE0);   // enable interrupt on timer 0 overflow
}

void SpeedControl::leftEngine(const Params p)
{
  g_left    =p;
  g_leftToGo=p.count_;
  EngSpeed::LeftEngine::set(p.speed_);
}

void SpeedControl::rightEngine(const Params p)
{
  g_right    =p;
  g_rightToGo=p.count_;
  EngSpeed::RightEngine::set(p.speed_);
}

void SpeedControl::stop(void)
{
  // pass stop orders
  const SpeedControl::Params p( 0, EngSpeed::Params(0,0) );
  leftEngine(p);
  rightEngine(p);
  // stop engines right away
  EngSpeed::stop();
}
