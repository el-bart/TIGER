/*
 * assert.h
 *
 */
#ifndef INCLUDE_ASSERT_H_FILE
#define INCLUDE_ASSERT_H_FILE

#ifndef NDEBUG
// debug

#include <inttypes.h>

void assert_internal_implementation(int cond, uint16_t line, const char *file);
#define assert(cond) assert_internal_implementation(cond, __LINE__, __FILE__)

#else
// release

#define assert(cond) do { } while(0)

#endif

#endif
