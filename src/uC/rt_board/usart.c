/*
 * usart.c
 *
 */
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "assert.h"
#include "usart.h"
#include "queue.h"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )


// TODO: add test for baud-rate error and show error if value is over 1%

// I/O queues
static struct queue_data in_q;
static struct queue_data out_q;

SIGNAL(SIG_UART_RECV)   // USART RX interrupt
{
  unsigned char c;
  c = UDR;
  // TODO
}

SIGNAL(SIG_UART_TRANS)  // USART TX interrupt
{
  // TODO
}

SIGNAL(SIG_UART_DATA)   // USART data register is empty interrupt
{
  // TODO
}


void usart_init(void)
{
  queue_init(&in_q);
  queue_init(&out_q);

  // clock devider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );
  // set single speed
  UCSRA =0x00;
  UCSRA&=~_BV(U2X);
  // TODO: this code is still broken.
goto test_this;

  // enable interrupts
  UCSRB|= _BV(RXCIE);   // RX complete
  UCSRB|= _BV(TXCIE);   // TX complete
  // TODO: this is invalid, since this interrupt loops forever
  //UCSRB|= _BV(UDRIE);   // data registry empty
  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure 8-bit transmition mode
  UCSRB&=~_BV(UCSZ2);   // 0
  UCSRC|= _BV(UCSZ1);   // 1
  UCSRC|= _BV(UCSZ0);   // 1
  // configure no parity mode
  UCSRC&=~_BV(UPM1);
  UCSRC&=~_BV(UPM0);
  // configure one stop bit
  UCSRC&=~_BV(USBS);

  // other configuration options
  UCSRC&=~_BV(URSEL);   // synchronous mode
  UCSRC&=~_BV(UCPOL);   // clock polarity mode

test_this:
  // Enable receiver and transmitter; enable RX interrupt
  UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
}

void usart_send(uint8_t b)
{
  // TODO
}

void usart_send_array(uint8_t *b, size_t size)
{
  assert(b!=NULL);
  for(size_t i=0; i<size; ++i)
    usart_send(b[i]);
}

size_t usart_inqueue_size(void)
{
  // TODO
  return 0;
}

uint8_t usart_receive(void)
{
  while( usart_inqueue_size()==0 ); // wait for the data
  // TODO
  return 0;
}
