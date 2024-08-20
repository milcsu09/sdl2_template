// #include "utils.h"
#include <SDL2/SDL_timer.h>
#include <stdarg.h>

void
log_implementation (FILE *stream, const char *__restrict fmt, ...)
{
  fprintf (stream, "%.3f\t", (float)SDL_GetTicks () / 1000);

  va_list va;
  va_start (va, fmt);
  vfprintf (stream, fmt, va);
  va_end (va);

  fprintf (stream, "\n");
}

