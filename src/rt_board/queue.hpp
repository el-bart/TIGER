/*
 * queue.h
 *
 */
#ifndef INCLUDE_QUEUE_HPP_FILE
#define INCLUDE_QUEUE_HPP_FILE

#include <stdio.h>
#include <inttypes.h>

#include "config.hpp"

struct queue_data
{
  uint8_t queue[QUEUE_MAX_SIZE];
  size_t  begin;
  size_t  size;
};

void queue_init(struct queue_data *q);

void queue_push(struct queue_data *q, uint8_t b);

size_t queue_size(struct queue_data *q);
int queue_full(struct queue_data *q);
int queue_empty(struct queue_data *q);

uint8_t queue_peek(struct queue_data *q, size_t pos);

uint8_t queue_pop(struct queue_data *q);

void queue_pop_count(struct queue_data *q, size_t n);

#endif
