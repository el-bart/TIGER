/*
 * uassert.h
 *
 */
#ifndef INCLUDE_UASSERT_H_FILE
#define INCLUDE_UASSERT_H_FILE

#ifndef NDEBUG
// debug

#include <inttypes.h>

void uassert_internal_implementation(int cond, uint16_t line, const char *file);
#define uassert(cond) uassert_internal_implementation(cond, __LINE__, __FILE__)

#else
// release

#define uassert(cond) do { } while(0)

#endif

#endif
