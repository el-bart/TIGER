/*
 * motors_control.c
 *
 */
#include "config.h"         // this file must be included as first one!

#include <avr/io.h>
//#include <util/delay.h>     // <avr/delay.h> once



//
// MAIN
//
int main(void)
{
  // one big TODO !

  DDRD=(1 << PB5);
  PORTD=_BV(5);


  for(;;)
  {
  }

  return 0;
} // main()

