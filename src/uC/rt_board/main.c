/*
 * motors_control.c
 *
 */
#include "config.h"         // this file must be included as first one!

#include <avr/io.h>
#include <util/delay.h>     // <avr/delay.h> once

#include "usart.h"
#include "assert.h"



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

  // enable inputs from foto
  DDRD&=~_BV(2);
  DDRD&=~_BV(3);
  PORTD|=_BV(2);
  PORTD|=_BV(3);

  usart_init();
  assert(0);

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

  // blink
  for(;;)
  {
    for(int i=0; i<4; ++i)
    {
      PORTD|=_BV(5);
      PORTC|=_BV(i);
      for(int i=0; i<4; ++i)
        _delay_ms(250);

      PORTD&=~_BV(5);
      PORTC&=~_BV(i);
      for(int i=0; i<4; ++i)
        _delay_ms(250);
    }
  }

  for(;;) { }

  return 0;
} // main()

