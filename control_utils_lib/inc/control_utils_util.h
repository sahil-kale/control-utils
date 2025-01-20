#ifndef UTIL_H
#define UTIL_H
#include <math.h>  // For fabsf()

#if __STDC_VERSION__ >= 201112L
/* C11 and above: Use _Static_assert directly */
#define CONTROL_UTILS_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
/* Pre-C11: Use negative-size array for compile-time assertion */
#define CONTROL_UTILS_STATIC_ASSERT(cond, msg) extern void static_assert_##__LINE__(int static_assert_failed[(cond) ? 1 : -1])
#endif

#define EPSILON 1e-6f  // Define a small tolerance for floating-point comparison

#define IS_APPROX_ZERO(x) (fabsf(x) < EPSILON)

#define CONTROL_UTILS_CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

#endif