#ifndef __BOREALGL_H__
#define __BOREALGL_H__

#include <SDL/SDL.h>

#define lock_screen(screen){if(SDL_MUSTLOCK(screen)){if(SDL_LockSurface(screen)<0){return -1;}}}
#define unlock_screen(screen){if(SDL_MUSTLOCK(screen)){SDL_UnlockSurface(screen);}SDL_UpdateRect(screen, 0, 0, 0, 0);}

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B);
void DrawLine(SDL_Surface *screen, int xd, int yd, int xf, int yf, Uint8 R, Uint8 G, Uint8 B);

#endif
