/*
 * queue.h
 *
 */
#ifndef INCLUDE_QUEUE_HPP_FILE
#define INCLUDE_QUEUE_HPP_FILE

#include <stdio.h>
#include <inttypes.h>

#include "config.hpp"

class Queue
{
public:
  Queue(void);

  void push(uint8_t b);

  size_t size(void) const;
  bool full(void) const
  {
    return size()==QUEUE_MAX_SIZE;
  }
  bool empty(void) const
  {
    return size()==0u;
  }

  uint8_t peek(size_t pos) const;
  uint8_t pop(void);
  void popCount(size_t n);

private:
  uint8_t queue_[QUEUE_MAX_SIZE];
  size_t  begin_;
  size_t  size_;
}; // class Queue

#endif
