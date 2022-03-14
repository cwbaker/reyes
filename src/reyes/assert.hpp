#pragma once

#include "config.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

void reyes_break( void );
void reyes_assert( int expression, const char* description, const char* file, int line );
void reyes_assert_with_break( int expression, const char* description, const char* file, int line );
   
#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#define REYES_BREAK() __debugbreak()
#elif defined(BUILD_OS_MACOSX)
#define REYES_BREAK() __asm__("int $3")
#else
#define REYES_BREAK() reyes_break()
#endif

#ifdef REYES_ASSERT_ENABLED

#define REYES_ASSERT( x ) \
do { \
    if ( !(x) ) \
    { \
        reyes_assert( false, #x, __FILE__, __LINE__ ); \
        REYES_BREAK(); \
    } \
} while ( false )

#else

#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif

#define REYES_ASSERT( x )

#endif
