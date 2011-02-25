/*
 * main.cpp
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>     // <avr/delay.h> once

#include "USART.hpp"
#include "Impulses.hpp"
#include "EngSpeed.hpp"
#include "SpeedControl.hpp"
#include "uassert.hpp"


// handle unknown interruption(s)
ISR(BADISR_vect)
{
  uassert(!"unhandled interrupt");
}


void sendHex(uint8_t v)
{
  uassert(v<16);
  const char *lut="0123456789abcdef";
  USART::send(lut[v]);
}

void sendChar(uint8_t v)
{
  sendHex((v&0xF0)>>4);
  sendHex((v&0x0F)>>0);
}

void sendV(uint16_t v)
{
  sendChar((v&0xFF00)>>8);
  sendChar((v&0x00FF)>>0);
}


void onLeftImpulse(const uint16_t c)
{
  onLeftImpulseControl(c);
}


void onRightImpulse(const uint16_t c)
{
  onRightImpulseControl(c);
}


//
// MAIN
//
int main(void)
{
  // one big TODO !

  // led
  DDRD|=(1 << PB5);

  // mode
  DDRC|=(1 << PC0);
  DDRC|=(1 << PC1);
  DDRC|=(1 << PC2);
  DDRC|=(1 << PC3);

  // enable
  DDRB|=(1 << PB1);
  DDRB|=(1 << PB2);

  // enable both
  PORTB|=(1<<PB1);
  PORTB|=(1<<PB2);

  USART::init();                                    // configure serial interface
  Impulses::init(onLeftImpulse, onRightImpulse);    // configure impulse counting via interrupts
  EngSpeed::init();                                 // engine speed control mechanism
  SpeedControl::init();                             // automatic speed control
  sei();                                            // enable interrupts globally

  while(true)
  {
    USART::receive();
    //const EngSpeed::Params ep(+1, 256/2);
    //EngSpeed::LeftEngine::set(ep);
    //EngSpeed::RightEngine::set(ep);
    //const SpeedControl::Params scp(0xFFFF, +1, 200);
    //SpeedControl::leftEngine(scp);
    //SpeedControl::rightEngine(scp);
    for(int i=0x10; i<0x60; i+=10)
    {
      USART::receive();
      SpeedControl::leftEngine(  SpeedControl::Params(0xFFFF, +1, i) );
      SpeedControl::rightEngine( SpeedControl::Params(0xFFFF, -1, 0x60-i) );
    }

    USART::receive();
    //EngSpeed::stop();
    SpeedControl::stop();
  }

  /*
  // output changes
  uint16_t left    =0;
  uint16_t right   =0;
  uint16_t leftOld =666;
  uint16_t rightOld=666;
  uint16_t cnt     =1;
  while(true)
  {
    left =Impulses::getLeftEngine();
    right=Impulses::getRightEngine();
    // nothing changed?
    if(left==leftOld && right==rightOld)
      continue;

    // ok - something has changed!
    sendV(cnt);
    USART::send(':');
    USART::send(' ');
    ++cnt;
    sendV(left);
    USART::send('/');
    sendV(right);
    USART::send('\n');

    // assign these values
    leftOld =left;
    rightOld=right;
  }
  */


  /*
  for(int i=0; i<10; ++i)
  {
    uint8_t c=USART::receive();
    USART::send(c+1);
    USART::send(c+2);
    USART::send(c+3);
    USART::send('\r');
    USART::send('\n');
  }

  uassert(!"TODO");
  */

  /*
  // forward state
  for(;;)
  {
    if( PIND&_BV(2) || PIND&_BV(3) )
    {
      PORTD|=_BV(5);
    }
    else
    {
      PORTD&=~_BV(5);
    }
  }
  */

  /*
  // blink
  for(;;)
  {
    for(int i=0; i<4; ++i)
    {
      PORTD|=_BV(5);
      PORTC|=_BV(i);
      for(int j=0; j<4; ++j)
        _delay_ms(250);

      PORTD&=~_BV(5);
      PORTC&=~_BV(i);
      for(int j=0; j<4; ++j)
        _delay_ms(250);
    }
  }
  */

  for(;;) { }

  return 0;
} // main()

