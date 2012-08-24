#include <tut/tut.hpp>
#include <thread>

#include "Parallel/Queue.hpp"

using namespace Parallel;

namespace
{
typedef Queue<std::string> StrQueue;

struct TestClass
{
  StrQueue q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Parallel/Queue");
} // unnamed namespace


namespace tut
{

// test initial state
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid initial size", q_.size(), 0);
  ensure("initial state is not empty", q_.empty() );
}


// test addition of an element
template<>
template<>
void testObj::test<2>(void)
{
  q_.push("doom");
  ensure_equals("invalid element", q_.pop(), "doom");
}


// test waiting for element
template<>
template<>
void testObj::test<3>(void)
{
  q_.push("doom");
  ensure("wait() failed", q_.waitForElementUntil( std::chrono::steady_clock::now() + std::chrono::milliseconds(1) ) );
  ensure_equals("invalid element", q_.pop(), "doom");
}


// test wait() timeout
template<>
template<>
void testObj::test<5>(void)
{
  ensure("wait() didn't failed", !q_.waitForElementUntil( std::chrono::steady_clock::now() + std::chrono::milliseconds(1) ) );
}


namespace
{
struct ThreadJoiner
{
  template<typename F>
  ThreadJoiner(F f): th_( std::move(f) ) { }
  ~ThreadJoiner(void) { th_.join(); }
  std::thread th_;
};
}

// test waiting while other thread adds elements
template<>
template<>
void testObj::test<6>(void)
{
  constexpr size_t count = 420;
  ThreadJoiner     th( [&]{ for(size_t i=0; i<count; ++i) q_.push("hello there"); } );

  for(size_t i=0; i<count; ++i)
  {
    ensure("wait() failed while waiting for element", q_.waitForElementUntil( std::chrono::steady_clock::now() + std::chrono::milliseconds(500) ) );
    ensure_equals("invalid element received", q_.pop(), "hello there");
  }
}

} // namespace tut
