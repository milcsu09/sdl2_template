#include "render.h"
#include "utils.h"
#include <SDL2/SDL.h>

int
main ()
{
  render_init ("Raycast", 800, 800, 4);

  for (;;)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event))
        {
          switch (event.type)
            {
            case SDL_QUIT:
              goto quit;
            default:
              break;
            }
        }

      render_clear (RENDER_PIXEL_BUFFER | RENDER_DEPTH_BUFFER);
      draw_point (20, 20, 0xFF0000, 0);
      render_present (SDL_FLIP_NONE);
    }

quit:
  render_quit ();
  return 0;
}

