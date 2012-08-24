#ifndef INCLUDE_PARALLEL_LOCABLE_HPP_FILE
#define INCLUDE_PARALLEL_LOCABLE_HPP_FILE

#include <mutex>

namespace Parallel
{

template<typename Parent, typename Mutex = std::mutex>
class Lockable
{
protected:
  Lockable(void) { }

public:
  typedef Mutex mutex_type;

  // lockable concept
  void lock(void) const { m_.lock(); }
  void unlock(void) const { m_.unlock(); }
  bool try_lock(void) const { return m_.try_lock(); }

  // get acess to mutex - needed in special cases...
  Mutex& mutex(void) { return m_; }

private:
  mutable Mutex m_;
};


template<typename Parent, typename Mutex = std::timed_mutex>
class TimedLockable
{
protected:
  TimedLockable(void) { }

public:
  typedef Mutex mutex_type;

  // lockable concept
  void lock(void) const { m_.lock(); }
  void unlock(void) const { m_.unlock(); }
  bool try_lock(void) const { return m_.try_lock(); }
  // timed-lockable concept
  template< class Rep, class Period >
  bool try_lock_for(const std::chrono::duration<Rep,Period>& timeout) const { return m_.try_lock_for(timeout); }
  template< class Clock, class Duration >
  bool try_lock_until(const std::chrono::time_point<Clock,Duration>& timeout) const { return m_.try_lock_until(timeout); }

  // get acess to mutex - needed in special cases...
  Mutex& mutex(void) { return m_; }

private:
  mutable Mutex m_;
};

}

#endif
