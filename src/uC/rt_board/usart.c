/*
 * usart.c
 *
 */
#include "usart.h"
#include "config.h"

void usart_init(void)
{
  // TODO
}

void usart_send(byte b)
{
  // TODO
}

void usart_send_array(byte *b, size_t size)
{
  if(b==NULL)
    return;
  for(size_t i=0; i<size; ++i)
    usart_send(b[i]);
}

size_t usart_inqueue_size(void)
{
  // TODO
  return 0;
}

byte usart_receive(void)
{
  while( usart_inqueue_size()==0 ); // wait for the data
  // TODO
  return 0;
}
