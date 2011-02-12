/*
 * usart.c
 *
 */
#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uassert.hpp"
#include "usart.hpp"
#include "queue.hpp"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

// TODO: add test for baud-rate error and show error if value is over 1%

//
// I/O queues
//
static struct queue_data g_in_q;
static struct queue_data g_out_q;


static inline void send_data_impl(void)
{
  uassert( UCSRA & _BV(UDRE) );
  UDR=queue_pop(&g_out_q);
}

// USART RX completed interrupt
ISR(USART_RXC_vect)
{
  const uint8_t c=UDR;      // read form hardware ASAP
  if( queue_full(&g_in_q) ) // if queue is full, drop last element
    queue_pop(&g_in_q);
  queue_push(&g_in_q, c);   // enqueue new byte
}

// USART TX completed interrupt
ISR(USART_TXC_vect)
{
  if( !queue_empty(&g_out_q) )  // if have something to send
    send_data_impl();
}

// USART data register is empty interrupt
ISR(USART_UDRE_vect)
{
  UCSRB&=~_BV(UDRIE);           // data registry empty - disable interrupt
  if( queue_size(&g_out_q)>0 )  // if data register is empty and we have data to send
    send_data_impl();           // we can send it now
}


void usart_init(void)
{
  queue_init(&g_in_q);
  queue_init(&g_out_q);

  // clock devider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );
  // set single speed
  UCSRA =0x00;
  UCSRA&=~_BV(U2X);

  // enable interrupts
  UCSRB|= _BV(RXCIE);   // RX complete
  UCSRB|= _BV(TXCIE);   // TX complete
  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure 8-bit transmition mode
  // NOTE: there is a bug in spec at this point - UCSZ[01] must be 0 not 1 here!
  UCSRB&=~_BV(UCSZ2);   // 0
  UCSRC&=~_BV(UCSZ1);   // 0 (!)
  UCSRC&=~_BV(UCSZ0);   // 0 (!)
  // configure no parity mode
  UCSRC&=~_BV(UPM1);
  UCSRC&=~_BV(UPM0);
  // configure one stop bit
  UCSRC&=~_BV(USBS);

  // other configuration options
  UCSRC&=~_BV(URSEL);   // synchronous mode
  UCSRC&=~_BV(UCPOL);   // clock polarity mode

  // configure proper pins as in (RX) and out (TX)
  DDRD &=~_BV(PD0);     // RX as in
  PORTD|= _BV(PD0);     // RX high with pull-up
  DDRD |= _BV(PD1);     // TX as out
}

void usart_send(uint8_t b)
{
  while( queue_full(&g_out_q) );    // wait for space in queue
  queue_push(&g_out_q, b);          // enqueue next char to send
  if( queue_size(&g_out_q)==1 )     // this might be first char to send
    UCSRB|=_BV(UDRIE);              // signal on data registry empty.
                                    // if transmition has not yet started this will
                                    // send initial (first) byte as soon as USART is ready
}

void usart_send_array(uint8_t *b, size_t size)
{
  uassert(b!=NULL);
  for(size_t i=0; i<size; ++i)
    usart_send(b[i]);
}

size_t usart_inqueue_size(void)
{
  return queue_size(&g_in_q);       // return input queue size
}

uint8_t usart_receive(void)
{
  while( queue_empty(&g_in_q) );    // wait for the data
  return queue_pop(&g_in_q);        // return read data
}
