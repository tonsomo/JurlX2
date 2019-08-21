/*
 font.c - a simple bitmap font engine
 
 (C) 2002 Ray Kelm
 This code is free to use without restrictions.
*/

#include "SDL.h"
#include "targa.h"
#include "font.h"

#include <stdio.h>
#include <string.h>

void Font_DrawText(SDL_Surface *screen, SDL_Surface *font, const char *s, int x, int y)
{
	SDL_Rect sr, dr;
	int n, i, max;
	int char_width;
	int char_height;
	
	if (x > screen->w || y > screen->h)
		return;

	char_width = font->w / 256;
	char_height = font->h;
	
	n = strlen(s);
	max = (screen->w - x) / char_width;
	if (n > max)
		n = max;

	dr.x = x;
	dr.y = y;
	dr.w = char_width;
	dr.h = char_height;
	sr = dr;
	sr.y = 0;

	for (i=0; i<n; i++)
	{
		sr.x = s[i] * char_width;
		SDL_BlitSurface(font, &sr, screen, &dr);
		dr.x += char_width;
	}
}

SDL_Surface *Font_Load(const char *filename)
{
	SDL_Surface *temp;
	SDL_Surface *font;
	
	temp = LoadTargaImage(filename);
	if (temp == 0)
	{
        fprintf(stderr,"Error Loading Font");
		return 0;
    }
    
	SDL_SetColorKey(temp, SDL_RLEACCEL | SDL_SRCCOLORKEY, 0);
	font = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	return font;
}
