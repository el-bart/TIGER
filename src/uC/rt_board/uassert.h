/*
 * uassert.h
 *
 */
#ifndef INCLUDE_UASSERT_H_FILE
#define INCLUDE_UASSERT_H_FILE

#ifndef NDEBUG
// debug

#include <inttypes.h>

void uassert_internal_implementation(uint16_t line, const char *file);
#define uassert(cond) do { if(!(cond)) uassert_internal_implementation(__LINE__, __FILE__); } while(0)

#else
// release

#define uassert(cond) do { } while(0)

#endif

#endif
