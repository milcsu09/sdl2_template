#include "render.h"
#include "utils.h"
#include <SDL2/SDL.h>

static struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  uint32_t *pixel_buffer;
  uint32_t *depth_buffer;
  uint32_t width, height;
} g_state = { 0 };

static size_t g_buffer_sz;

void
render_init (const char *title, uint32_t width, uint32_t height,
             uint32_t pixel_size)
{
  LOG_DEBUG ("Initializing SDL2!");
  ASSERT (SDL_Init (SDL_INIT_VIDEO) == 0, "SDL_Init(): %s\n",
          SDL_GetError ());

  g_state.window = SDL_CreateWindow (title, SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, width,
                                     height, SDL_WINDOW_SHOWN);
  ASSERT (g_state.window, "SDL_CreateWindow(): %s\n",
          SDL_GetError ());

  g_state.renderer = SDL_CreateRenderer (g_state.window, -1,
                                         SDL_RENDERER_ACCELERATED);
  ASSERT (g_state.window, "SDL_CreateRenderer(): %s\n",
          SDL_GetError ());

  g_state.width = width / pixel_size;
  g_state.height = height / pixel_size;

  g_state.texture = SDL_CreateTexture (
      g_state.renderer, SDL_PIXELFORMAT_RGB888,
      SDL_TEXTUREACCESS_STREAMING, g_state.width, g_state.height);
  ASSERT (g_state.texture, "SDL_CreateTexture(): %s\n",
          SDL_GetError ());

  g_buffer_sz = g_state.width * g_state.height * sizeof (uint32_t);

  const float buffer_sz_mb = g_buffer_sz / 1024.0f / 1024.0f;
  LOG_DEBUG ("Allocating 'pixel_buffer' (%.2f Mb)!", buffer_sz_mb);
  LOG_DEBUG ("Allocating 'depth_buffer' (%.2f Mb)!", buffer_sz_mb);

  g_state.pixel_buffer = malloc (g_buffer_sz);
  g_state.depth_buffer = malloc (g_buffer_sz);

  LOG_DEBUG ("Done!");
}

void
render_quit ()
{
  LOG_DEBUG ("Goodbye!");
  SDL_DestroyTexture (g_state.texture);
  SDL_DestroyRenderer (g_state.renderer);
  SDL_DestroyWindow (g_state.window);
  free (g_state.pixel_buffer);
  free (g_state.depth_buffer);
  SDL_Quit ();
}

void
render_clear (uint64_t flags)
{
  if (flags & RENDER_PIXEL_BUFFER)
    memset (g_state.pixel_buffer, 0xFF, g_buffer_sz);

  if (flags & RENDER_DEPTH_BUFFER)
    memset (g_state.depth_buffer, 0xFF, g_buffer_sz);
}

void
render_present (int64_t flags)
{
  SDL_UpdateTexture (g_state.texture, NULL, g_state.pixel_buffer,
                     g_state.width * sizeof (uint32_t));
  SDL_RenderCopyEx (g_state.renderer, g_state.texture, NULL, NULL, 0,
                    NULL, flags);
  SDL_RenderPresent (g_state.renderer);
}

static inline uint32_t
point_to_index (float x, float y, uint32_t stride)
{
  return (int32_t)x + (int32_t)y * stride;
}

static inline uint8_t
point_in_bounds (float x, float y)
{
  return (x >= 0 && x < g_state.width)
         && (y >= 0 && y < g_state.height);
}

void
draw_point (float x, float y, uint32_t color, uint32_t depth)
{
  if (!point_in_bounds (x, y))
    return;

  const uint32_t index = point_to_index (x, y, g_state.width);
  if (depth < g_state.depth_buffer[index])
    {
      g_state.pixel_buffer[index] = color;
      g_state.depth_buffer[index] = depth;
    }
}
