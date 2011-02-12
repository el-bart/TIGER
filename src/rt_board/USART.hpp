/*
 * usart.h
 *
 */
#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include <stdio.h>
#include <inttypes.h>

namespace USART
{

void init(void);

void send(uint8_t b);

void send_array(uint8_t *b, size_t size);

size_t inqueue_size(void);

uint8_t receive(void);

} // unamespace USART

#endif
