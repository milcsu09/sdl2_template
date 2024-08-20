#ifndef ENGINE_RENDER_H
#define ENGINE_RENDER_H

#include <stdint.h>

enum
{
  RENDER_PIXEL_BUFFER = (1 << 0),
  RENDER_DEPTH_BUFFER = (1 << 1),
};

extern void render_init (const char *, uint32_t, uint32_t, uint32_t);
extern void render_quit ();

extern void render_clear (uint64_t);
extern void render_present (int64_t);

extern void draw_point (float, float, uint32_t, uint32_t);

#endif // ENGINE_RENDER_H

