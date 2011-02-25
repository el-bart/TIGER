/*
 * SpeedControl.cpp
 *
 */
#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "SpeedControl.hpp"
#include "EngSpeed.hpp"
#include "uassert.hpp"

namespace
{
SpeedControl::Params g_left(0,0,0);
uint16_t             g_leftToGo=0;

SpeedControl::Params g_right(0,0,0);
uint16_t             g_rightToGo=0;


inline uint16_t delayToExpected(const SpeedControl::Params &cfg)
{
  return cfg.delay_;//*8;
} // delayToExpected()

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
// when last update has been performed
Timestamp g_lastLeftUpdate;
Timestamp g_lastRightUpdate;


template<typename TEng>
inline void impulseHandle(Timestamp &lastImpulse, Timestamp &lastUpdate, uint16_t &impulsesToGo, const SpeedControl::Params &cfg)
{
  const Timestamp now;              // save this time ASAP
  const Timestamp last=lastImpulse; // save in temporary variable
  lastImpulse=now;                  // update real variable

  if(impulsesToGo==0)
  {
    TEng::stop();
    // TODO: callback for the next orders?
    return;
  }
  --impulsesToGo;

  const uint16_t diff    =now-last;
  const uint16_t expected=delayToExpected(cfg);
  // simple +/- control
  if( now-lastUpdate > 20 )
  {
    if(diff==expected)
      return;
    if(diff<expected)
      TEng::modify(-1);
    else
      TEng::modify(+1);
    lastUpdate=now;
  }
} // impulseHandle()


template<typename TEng>
inline void checkNoImpulse(const Timestamp &now,        const Timestamp &lastImpulse, Timestamp &lastUpdate,
                           const uint16_t impulsesToGo, const SpeedControl::Params &cfg)
{
  // if no impulses are left, do nothing
  if(impulsesToGo==0)
    return;
  if( now-lastUpdate > 150 )
  {
    // no impulse for some time?
    const uint16_t diff    =now-lastImpulse;
    const uint16_t expected=delayToExpected(cfg);
    if(diff>expected)
      TEng::modify(+5);        // give it extra power!
    lastUpdate=now;
  }
} // checkNoImpulse
} // unnamed namespace


// timer 0 interrupt
ISR(TIMER0_OVF_vect)
{
  ++g_timerCycles;
  const Timestamp now;
  checkNoImpulse<EngSpeed::LeftEngine>( now, g_lastLeftImpulse,  g_lastLeftUpdate,  g_leftToGo,  g_left );
  checkNoImpulse<EngSpeed::RightEngine>(now, g_lastRightImpulse, g_lastRightUpdate, g_rightToGo, g_right);
}


void onLeftImpulseControl(uint16_t c)
{
  impulseHandle<EngSpeed::LeftEngine>(g_lastLeftImpulse, g_lastLeftUpdate, g_leftToGo, g_left);
}

void onRightImpulseControl(uint16_t c)
{
  impulseHandle<EngSpeed::RightEngine>(g_lastRightImpulse, g_lastRightUpdate, g_rightToGo, g_right);
}


void SpeedControl::init(void)
{
  // assign initial values
  g_leftToGo   =0;
  g_rightToGo  =0;
  g_timerCycles=0;

  // set timer 0
  TCNT0 =0;             // start counting from 0
  TCCR0|= _BV(CS02);    // set prescaller to clk/1024
  TCCR0&=~_BV(CS01);    // ...
  TCCR0|= _BV(CS00);    // ...
  TIMSK|= _BV(TOIE0);   // enable interrupt on timer 0 overflow
}

void SpeedControl::leftEngine(const Params p)
{
  g_left    =p;
  g_leftToGo=p.count_;
  uint8_t pwm=EngSpeed::LeftEngine::get().value_;
  if(pwm==0)
    pwm=80;
  EngSpeed::LeftEngine::set( EngSpeed::Params(p.dir_, pwm) );
}

void SpeedControl::rightEngine(const Params p)
{
  g_right    =p;
  g_rightToGo=p.count_;
  uint8_t pwm=EngSpeed::LeftEngine::get().value_;
  if(pwm==0)
    pwm=80;
  EngSpeed::RightEngine::set( EngSpeed::Params(p.dir_, pwm) );
}

void SpeedControl::stop(void)
{
  // pass stop orders
  const SpeedControl::Params p(0,0,0);
  leftEngine(p);
  rightEngine(p);
  // stop engines right away
  EngSpeed::stop();
}
