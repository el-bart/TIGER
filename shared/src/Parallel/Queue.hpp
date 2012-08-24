#ifndef INCLUDE_PARALLEL_QUEUE_HPP_FILE
#define INCLUDE_PARALLEL_QUEUE_HPP_FILE

#include <queue>
#include <mutex>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <condition_variable>
#include <cassert>


namespace Parallel
{

template<typename T>
class Queue
{
private:
  // NOTE: deriving from TimedLockable cannot be the case since deadlock bug in gcc 4.7.1
  //       and before on condition_variable with user-defined lock type. because of this
  //       some more bagic is needed and we're limiting ourselves to holding movable objects
  //       so that it could be assumed that returning by value will never throw.
  static_assert( std::is_move_constructible<T>::value, "elements must be movable for this container to work reliably" );
  static_assert( std::is_move_assignable<T>::value, "elements must be movable for this container to work reliably" );

  typedef std::unique_lock<std::mutex> Lock;

public:
  void push(T t)
  {
    Lock l(m_);
    q_.push( std::move(t) );
    nonEmpty_.notify_one();
  }

  T pop(void)
  {
    Lock l(m_);
    if( q_.empty() )
      throw std::logic_error("requesting element from the empty collection");
    T tmp = std::move( q_.front() );
    q_.pop();
    return tmp;
  }

  size_t size(void) const
  {
    Lock l(m_);
    return q_.size();
  }

  bool empty(void) const
  {
    Lock l(m_);
    return q_.empty();
  }

  template<typename Duration>
  bool waitForElementUntil(const Duration& timeout) const
  {
    Lock l(m_);
    while( q_.empty() )
      if( nonEmpty_.wait_until(l, timeout) == std::cv_status::timeout )
        return false;
    assert( q_.empty() == false );
    return true;
  }

private:
  std::queue<T>                   q_;
  mutable std::mutex              m_;
  mutable std::condition_variable nonEmpty_;
};

}

#endif
