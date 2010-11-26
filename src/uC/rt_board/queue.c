/*
 * queue.c
 *
 */
#include <util/atomic.h>
#include <assert.h>

#include "queue.h"

void queue_init(struct queue_data *q)
{
  assert(q!=NULL);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    q->begin=0;
    q->size =0;
  }
}

void queue_push(struct queue_data *q, uint8_t b)
{
  assert(q!=NULL);
  assert( !queue_full(q) );
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    size_t pos=q->begin+q->size;
    if(pos>=QUEUE_MAX_SIZE)
      pos-=QUEUE_MAX_SIZE;
    q->queue[pos]=b;
    ++q->size;
  }
}

size_t queue_size(struct queue_data *q)
{
  size_t tmp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    tmp=q->size;
  }
  assert(tmp<=QUEUE_MAX_SIZE);
  return tmp;
}

int queue_full(struct queue_data *q)
{
  return queue_size(q)==QUEUE_MAX_SIZE;
}

int queue_empty(struct queue_data *q)
{
  return queue_size(q)==0;
}

uint8_t queue_peek(struct queue_data *q, size_t pos)
{
  uint8_t tmp;
  assert(q!=NULL);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    assert(q->size>pos);
    size_t qpos=q->begin+pos;
    if(qpos>=QUEUE_MAX_SIZE)
      qpos-=QUEUE_MAX_SIZE;
    tmp=q->queue[qpos];
  }
  return tmp;
}

uint8_t queue_pop(struct queue_data *q)
{
  assert(q!=NULL);
  assert( !queue_empty(q) );
  uint8_t tmp;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    tmp=q->queue[q->begin];
    // move the begining
    ++q->begin;
    if(q->begin==QUEUE_MAX_SIZE)
      q->begin=0;
    // update size
    --q->size;
  }
  return tmp;
}

void queue_pop_count(struct queue_data *q, size_t n)
{
  assert(q!=NULL);
  for(size_t i=0; i<n; ++i)
    queue_pop(q);
}
