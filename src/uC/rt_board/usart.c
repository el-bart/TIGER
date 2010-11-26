/*
 * usart.c
 *
 */
#include <assert.h>

#include "usart.h"
#include "config.h"

// TODO: add test for baud-rate error and show error if value is over 1%

void usart_init(void)
{
  // TODO
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
