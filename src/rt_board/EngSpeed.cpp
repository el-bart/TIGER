/*
 * EngSpeed.cpp
 *
 */
#include "config.hpp"
#include <avr/io.h>

#include "EngSpeed.hpp"
#include "uassert.hpp"

namespace
{
void setDirection(const char dir, const int pinForward, const int pinBackward)
{
  if(dir==0)
  {
    PORTC&=~_BV(pinForward);
    PORTC&=~_BV(pinBackward);
    return;
  }
  if(dir>0)
  {
    PORTC|= _BV(pinForward);
    PORTC&=~_BV(pinBackward);
    return;
  }
  if(dir<0)
  {
    PORTC&=~_BV(pinForward);
    PORTC|= _BV(pinBackward);
    return;
  }
  uassert(!"code never reaches here");
} // setDirection()

char getDirection(const int pinForward, const int pinBackward)
{
  const bool fw=PORTC&_BV(pinForward);
  const bool bw=PORTC&_BV(pinBackward);
  if(fw==bw)
    return 0;
  if(fw)
    return 1;
  if(bw)
    return -1;
  uassert(!"code never reaches here");
  return 0;     // just for the compiler to back off
} // getDirection()
} // unnamed namespace


void EngSpeed::init(void)
{
  // configure engine direction pins as outputs
  DDRC|=_BV(PC0);   // left:  backward
  DDRC|=_BV(PC1);   // left:  forward
  DDRC|=_BV(PC2);   // right: backward
  DDRC|=_BV(PC3);   // right: forward

  // left engine enable as PWM / OC1A
  setLeftEngine( Params(0, 0) );    // stop engine at the begining
  DDRB  |= _BV(PB1);                // configure output pin
  TCCR1A|= _BV(COM1A1);             // clear on TOP, set on MAX
  TCCR1A&=~_BV(COM1A0);             // ...
  TCCR1A&=~_BV(FOC1A);              // turn off force-output-compare

  // right engine enable as PWM / OC1B
  setRightEngine( Params(0, 0) );   // stop engine at the begining
  DDRB  |= _BV(PB2);                // configure output pin
  TCCR1A|= _BV(COM1B1);             // clear on TOP, set on MAX
  TCCR1A&=~_BV(COM1B0);             // ...
  TCCR1A&=~_BV(FOC1B);              // turn off force-output-compare

  // configure timer
  TCCR1B|= _BV(WGM13);              // fast-PWM mode, TOP==ICR1
  TCCR1B|= _BV(WGM12);              // ...
  TCCR1A|= _BV(WGM11);              // ...
  TCCR1A&=~_BV(WGM10);              // ...
  // NOTE: following settings gives 8MHz/2^16==122Hz frequency,with variable fill-out.
  // configure TOP to MAX
  ICR1H=0xFF;
  ICR1L=0xFF;
#if(F_CPU!=8*MHz)
#error TODO: CPU frequency has been changed, thus prescaler needs to be re-calbibrated
#endif
  TCCR1B&=~_BV(CS12);               // set prescaler to clk/1
  TCCR1B&=~_BV(CS11);               // ...
  TCCR1B|= _BV(CS10);               // ...
}

EngSpeed::Params EngSpeed::getLeftEngine(void)
{
  return Params(getDirection(PC1, PC0), OCR1AH);
}

void EngSpeed::setLeftEngine(const Params p)
{
  setDirection(p.dir_, PC1, PC0);
  OCR1AH=p.value_;
  OCR1AL=p.value_;
}

EngSpeed::Params EngSpeed::getRightEngine(void)
{
  return Params( getDirection(PC1, PC0), OCR1BH);
}

void EngSpeed::setRightEngine(const Params p)
{
  setDirection(p.dir_, PC3, PC2);
  OCR1BH=p.value_;
  OCR1BL=p.value_;
}

void EngSpeed::stop(void)
{
  const Params p(0, 0);
  setLeftEngine(p);
  setRightEngine(p);
}
