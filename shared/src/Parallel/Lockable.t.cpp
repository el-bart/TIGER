#include <tut/tut.hpp>

#include "Parallel/Lockable.hpp"

using namespace Parallel;

namespace
{
struct SomeData: public Lockable<SomeData>
{
};

struct SomeData2: public TimedLockable<SomeData2>
{
};


struct TestClass
{
  const SomeData  sd_;
  const SomeData2 sd2_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Parallel/Lockable");
} // unnamed namespace


namespace tut
{

// test lock/unlock
template<>
template<>
void testObj::test<1>(void)
{
  sd_.lock();
  sd_.unlock();
}


// test try_lock
template<>
template<>
void testObj::test<2>(void)
{
  const bool res = sd_.try_lock();
  if(!res)
    fail("mutex was not locked. in general this may happen, but should not in this case");
  sd_.unlock();
}


// test timed lock for - positive case
template<>
template<>
void testObj::test<3>(void)
{
  ensure("unable to lock", sd2_.try_lock_for( std::chrono::milliseconds(1) ) );
  sd2_.unlock();
}


// test timed lock for - negative case
template<>
template<>
void testObj::test<4>(void)
{
  sd2_.lock();
  ensure("lock didn't failed", !sd2_.try_lock_for( std::chrono::milliseconds(1) ) );
  sd2_.unlock();
}


// test timed lock until - positive case
template<>
template<>
void testObj::test<5>(void)
{
  ensure("unable to lock", sd2_.try_lock_until( std::chrono::steady_clock::now() + std::chrono::milliseconds(1) ) );
  sd2_.unlock();
}


// test timed lock until - negative case
template<>
template<>
void testObj::test<6>(void)
{
  sd2_.lock();
  ensure("lock didn't failed", !sd2_.try_lock_until( std::chrono::steady_clock::now() + std::chrono::milliseconds(1) ) );
  sd2_.unlock();
}

} // namespace tut
