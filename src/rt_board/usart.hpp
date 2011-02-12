/*
 * usart.h
 *
 */
#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include <stdio.h>
#include <inttypes.h>

void usart_init(void);

void usart_send(uint8_t b);

void usart_send_array(uint8_t *b, size_t size);

size_t usart_inqueue_size(void);

uint8_t usart_receive(void);

#endif
