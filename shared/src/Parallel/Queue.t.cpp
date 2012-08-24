#include <tut/tut.hpp>

#include "Parallel/Queue.hpp"

using namespace Parallel;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Parallel/Queue");
} // unnamed namespace


namespace tut
{

// 
template<>
template<>
void testObj::test<1>(void)
{
}


// 
template<>
template<>
void testObj::test<2>(void)
{
}


// 
template<>
template<>
void testObj::test<3>(void)
{
}

} // namespace tut
