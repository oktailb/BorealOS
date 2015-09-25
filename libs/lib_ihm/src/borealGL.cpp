#include <borealGL.hh>

#include <SDL_endian.h> 

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    Uint32 color = SDL_MapRGB(screen->format, R, G, B);
    switch (screen->format->BytesPerPixel) {
        case 1: {
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
            *bufp = color;
        }
        break;
        case 2: { 
            Uint16 *bufp;

            bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
            *bufp = color;
        }
        break;
        case 3: {
            Uint8 *bufp;

            bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
            if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                bufp[0] = color;
                bufp[1] = color >> 8;
                bufp[2] = color >> 16;
            } else {
                bufp[2] = color;
                bufp[1] = color >> 8;
                bufp[0] = color >> 16;
            }
        }
        break;
        case 4: {
            Uint32 *bufp;

            bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
}

void DrawLine(SDL_Surface* screen, int xd, int yd, int xf, int yf, Uint8 R, Uint8 G, Uint8 B) {
        int dy = yf - yd;
        int dx = xf - xd;
        int stepx, stepy;

        if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
        if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }

        DrawPixel(screen, xd, yd, R, G, B);
        DrawPixel(screen, xf, yf, R, G, B);
        if (dx > dy) {
            int length = (dx - 1) >> 2;
            int extras = (dx - 1) & 3;
            int incr2 = (dy << 2) - (dx << 1);
            if (incr2 < 0) {
                int c = dy << 1;
                int incr1 = c << 1;
                int d =  incr1 - dx;
                for (int i = 0; i < length; i++) {
                    xd += stepx;
                    xf -= stepx;
                    if (d < 0) {                                     // Pattern:
                        DrawPixel(screen, xd, yd, R, G, B);                    //
                        DrawPixel(screen, xd += stepx, yd, R, G, B);                 //  x o o
                        DrawPixel(screen, xf, yf, R, G, B);                          //
                        DrawPixel(screen, xf -= stepx, yf, R, G, B);
                        d += incr1;
                    } else {
                        if (d < c) {                                 // Pattern:
                            DrawPixel( screen, xd, yd, R, G, B);                      //      o
                            DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);    //  x o
                            DrawPixel( screen, xf, yf, R, G, B);                      //
                            DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                        } else {
                            DrawPixel( screen, xd, yd += stepy, R, G, B);             // Pattern:
                            DrawPixel( screen, xd += stepx, yd, R, G, B);             //    o o 
                            DrawPixel( screen, xf, yf -= stepy, R, G, B);             //  x
                            DrawPixel( screen, xf -= stepx, yf, R, G, B);             //
                        }
                        d += incr2;
                    }
                }
                if (extras > 0) {
                    if (d < 0) {
                        DrawPixel( screen, xd += stepx, yd, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf -= stepx, yf, R, G, B);
                    } else
                    if (d < c) {
                        DrawPixel(screen, xd += stepx, yd, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf -= stepx, yf, R, G, B);
                    } else {
                        DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                    }
                }
            } else {
                int c = (dy - dx) << 1;
                int incr1 = c << 1;
                int d =  incr1 + dx;
                for (int i = 0; i < length; i++) {
                    xd += stepx;
                    xf -= stepx;
                    if (d > 0) {
                        DrawPixel( screen, xd, yd += stepy, R, G, B);                      // Pattern:
                        DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);             //      o
                        DrawPixel( screen, xf, yf -= stepy, R, G, B);                      //    o
                        DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);	          //  x
                        d += incr1;
                    } else {
                        if (d < c) {
                            DrawPixel( screen, xd, yd, R, G, B);                           // Pattern:
                            DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);         //      o
                            DrawPixel( screen, xf, yf, R, G, B);                           //  x o
                            DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);         //
                        } else {
                            DrawPixel( screen, xd, yd += stepy, R, G, B);                  // Pattern:
                            DrawPixel( screen, xd += stepx, yd, R, G, B);                  //    o o
                            DrawPixel( screen, xf, yf -= stepy, R, G, B);                  //  x
                            DrawPixel( screen, xf -= stepx, yf, R, G, B);                  //
                        }
                        d += incr2;
                    }
                }
                if (extras > 0) {
                    if (d > 0) {
                        DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                    } else
                    if (d < c) {
                        DrawPixel( screen, xd += stepx, yd, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf -= stepx, yf, R, G, B);
                    } else {
                        DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd, R, G, B);
                        if (extras > 2) {
                            if (d > c)
                                DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                            else
                                DrawPixel( screen, xf -= stepx, yf, R, G, B);
                        }
                    }
                }
            }
        } else {
            int length = (dy - 1) >> 2;
            int extras = (dy - 1) & 3;
            int incr2 = (dx << 2) - (dy << 1);
            if (incr2 < 0) {
                int c = dx << 1;
                int incr1 = c << 1;
                int d =  incr1 - dy;
                for (int i = 0; i < length; i++) {
                    yd += stepy;
                    yf -= stepy;
                    if (d < 0) {
                        DrawPixel( screen, xd, yd, R, G, B);
                        DrawPixel( screen, xd, yd += stepy, R, G, B);
                        DrawPixel( screen, xf, yf, R, G, B);
                        DrawPixel( screen, xf, yf -= stepy, R, G, B);
                        d += incr1;
                    } else {
                        if (d < c) {
                            DrawPixel( screen, xd, yd, R, G, B);
                            DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                            DrawPixel( screen, xf, yf, R, G, B);
                            DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                        } else {
                            DrawPixel( screen, xd += stepx, yd, R, G, B);
                            DrawPixel( screen, xd, yd += stepy, R, G, B);
                            DrawPixel( screen, xf -= stepx, yf, R, G, B);
                            DrawPixel( screen, xf, yf -= stepy, R, G, B);
                        }
                        d += incr2;
                    }
                }
                if (extras > 0) {
                    if (d < 0) {
                        DrawPixel( screen, xd, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf, yf -= stepy, R, G, B);
                    } else
                    if (d < c) {
                        DrawPixel( screen, stepx, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf, yf -= stepy, R, G, B);
                    } else {
                        DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                    }
                }
            } else {
                int c = (dx - dy) << 1;
                int incr1 = c << 1;
                int d =  incr1 + dy;
                for (int i = 0; i < length; i++) {
                    yd += stepy;
                    yf -= stepy;
                    if (d > 0) {
                        DrawPixel( screen, xd += stepx, yd, R, G, B);
                        DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        DrawPixel( screen, xf -= stepx, yf, R, G, B);
                        DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                        d += incr1;
                    } else {
                        if (d < c) {
                            DrawPixel( screen, xd, yd, R, G, B);
                            DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                            DrawPixel( screen, xf, yf, R, G, B);
                            DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                        } else {
                            DrawPixel( screen, xd += stepx, yd, R, G, B);
                            DrawPixel( screen, xd, yd += stepy, R, G, B);
                            DrawPixel( screen, xf -= stepx, yf, R, G, B);
                            DrawPixel( screen, xf, yf -= stepy, R, G, B);
                        }
                        d += incr2;
                    }
                }
                if (extras > 0) {
                    if (d > 0) {
                        DrawPixel(screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                    } else
                    if (d < c) {
                        DrawPixel(screen, xd, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 2) DrawPixel( screen, xf, yf -= stepy, R, G, B);
                    } else {
                        DrawPixel(screen, xd += stepx, yd += stepy, R, G, B);
                        if (extras > 1) DrawPixel( screen, xd, yd += stepy, R, G, B);
                        if (extras > 2) {
                            if (d > c)
                                DrawPixel( screen, xf -= stepx, yf -= stepy, R, G, B);
                            else
                                DrawPixel( screen, xf, yf -= stepy, R, G, B);
                        }
                    }
                }
            }
        }

}
