/*
 * usart.h
 *
 */
#ifndef INCLUDE_USART_H_FILE
#define INCLUDE_USART_H_FILE

#include <stdio.h>
#include <inttypes.h>

typedef uint8_t byte;


void usart_init(void);

void usart_send(byte b);

void usart_send_array(byte *b, size_t size);

size_t usart_inqueue_size(void);

byte usart_receive(void);

#endif
