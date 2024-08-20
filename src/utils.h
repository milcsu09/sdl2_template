#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <stdio.h>

#define ASSERT(expr, ...)                                            \
  if (!(expr))                                                       \
  exit ((LOG_ERROR (__VA_ARGS__), 1))

#define LOG_DEBUG(...) log_implementation (stdout, __VA_ARGS__)
#define LOG_ERROR(...) log_implementation (stderr, __VA_ARGS__)

extern void log_implementation (FILE *, const char *__restrict, ...);

#endif // ENGINE_UTILS_H
