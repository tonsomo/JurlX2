/*
 targa.h - header for targa.c, a simple targa file reader
 
 (C) 2002 Ray Kelm
 This code is free to use without restrictions.
*/

#include "SDL.h"

#ifndef TARGA_H
#define TARGA_H

#ifdef __cplusplus
extern "C" {
#endif

SDL_Surface *LoadTargaImage(const char *filename);

#ifdef __cplusplus
};
#endif

#endif
