/*
 * usart.h
 *
 */
#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include <stdio.h>
#include <inttypes.h>

struct USART
{
  static void init(void);

  static void send(uint8_t b);

  static void send(uint8_t *b, size_t size);

  static size_t inQueueSize(void);

  static uint8_t receive(void);
}; // struct USART

#endif
