/*
 font.h - header for font.c, a simple bitmap font engine
 
 (C) 2002 Ray Kelm
 This code is free to use without restrictions.
*/

#include "SDL.h"

#ifndef FONT_H
#define FONT_H

SDL_Surface *Font_Load(const char *filename);
void Font_DrawText(SDL_Surface *screen, SDL_Surface *font, const char *s, int x, int y);

#endif
