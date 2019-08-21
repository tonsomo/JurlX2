/*
 targa.c - a simple targa file reader
 
 (C) 2005 Ray Kelm
 This code is free to use without restrictions.
*/

#include <stdio.h>

#include "SDL.h"
#include "targa.h"

#define TARGA_HEADER_SIZE   18

#define TARGA_ID_LENGTH      0
#define TARGA_COLORMAP_TYPE  1
#define TARGA_IMAGE_TYPE     2

#define TARGA_COLOR_SPEC     3 // 5 bytes

#define TARGA_IMAGE_SPEC     8 // 10 bytes
#define TARGA_IMAGE_WIDTH    12
#define TARGA_IMAGE_HEIGHT   14
#define TARGA_IMAGE_DEPTH    16
#define TARGA_IMAGE_DESCRIPTOR 17

#define ORIGIN_MASK         0x30
#define ORIGIN_BOTTOM_LEFT  0x00
#define ORIGIN_BOTTOM_RIGHT 0x10
#define ORIGIN_TOP_LEFT     0x20
#define ORIGIN_TOP_RIGHT    0x30

SDL_Surface *LoadTargaImage(const char *filename)
{
	FILE *fp;
	Uint8 targaHeader[TARGA_HEADER_SIZE];
	int width;
	int height;
	int depth;
	SDL_Surface *image;
	int bytes_per_pixel;
	Uint8 b;
	Uint8 targa_pixel[4];
	int n;
	int i;
	Uint8 *pixel;
	int origin;
	int x=0;
	int y=0;

	fp = fopen(filename, "rb");
	if (fp == 0)
	{
		fprintf(stderr, "LoadTargaImage: could not open file %s\n", filename);
		return 0;
	}

	fread(targaHeader, TARGA_HEADER_SIZE, 1, fp);

	// color maps are not supported
	if (targaHeader[TARGA_COLORMAP_TYPE] != 0)
	{
		fclose(fp);
		fprintf(stderr, "LoadTargaImage: unsupported colormap type (%d)", targaHeader[TARGA_COLORMAP_TYPE]);
		return 0;
	}

	// image formats other than RLE TrueColor are not supported
	if (targaHeader[TARGA_IMAGE_TYPE] != 10)
	{
		fclose(fp);
		fprintf(stderr, "LoadTargaImage: unsupported image type (%d)", targaHeader[TARGA_IMAGE_TYPE]);
		return 0;
	}

	width = targaHeader[TARGA_IMAGE_WIDTH] + targaHeader[TARGA_IMAGE_WIDTH+1] * 256;
	height = targaHeader[TARGA_IMAGE_HEIGHT] + targaHeader[TARGA_IMAGE_HEIGHT+1] * 256;
	depth = targaHeader[TARGA_IMAGE_DEPTH];
	origin = targaHeader[TARGA_IMAGE_DESCRIPTOR] & ORIGIN_MASK;
	
	// image origin other than top-left not supported
	if (origin != ORIGIN_TOP_LEFT && origin != ORIGIN_BOTTOM_LEFT)
	{
		fclose(fp);
		fprintf(stderr, "LoadTargaImage: unsupported image origin");
		return 0;
	}

	// image depth other than 24 or 32 are not supported
	if (depth != 32 && depth != 24)
	{
		fclose(fp);
		fprintf(stderr, "LoadTargaImage: unsupported image depth (%d)\n", depth);
		return 0;
	}

	image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		0x000000FF, 0x0000FF00, 0x00FF0000, (depth == 32) ? 0xFF000000 : 0);
#else
		0xFF000000, 0x00FF0000, 0x0000FF00, (depth == 32) ? 0x000000FF : 0);
#endif
	if (image == 0)
	{
		fclose(fp);
		fprintf(stderr, "LoadTargaImage: failed to create image surface: %s\n", SDL_GetError());
		return 0;
	}

	// skip image id
	fseek(fp, targaHeader[TARGA_ID_LENGTH], SEEK_CUR);

	bytes_per_pixel = (depth == 32) ? 4 : 3;
	n = width * height;

	pixel = (Uint8 *) image->pixels;

	switch (origin)
	{
		case ORIGIN_TOP_LEFT:
			x = 0;
			y = 0;
			pixel = (Uint8 *) image->pixels;
			break;			
		case ORIGIN_BOTTOM_LEFT:
			x = 0;
			y = height-1;
			pixel = (Uint8 *) image->pixels + y * 4 * width;
			break;
	}

	while (n)
	{
		fread(&b, 1, 1, fp);
		if (b & 0x80)
		{
			// run-length packet
			fread(targa_pixel, bytes_per_pixel, 1, fp);
			b -= 0x80;
			for (i=0; i<=b; i++)
			{
				pixel[0] = targa_pixel[2];
				pixel[1] = targa_pixel[1];
				pixel[2] = targa_pixel[0];
				if (depth == 32)
					pixel[3] = targa_pixel[3];
				else
					pixel[3] = 0xFF;
				if (origin == ORIGIN_BOTTOM_LEFT)
				{
					x++;
					if (x >= width)
					{
						x = 0;
						y--;
					}
				}
				else
				{
					x++;
					if (x >= width)
					{
						x = 0;
						y++;
					}
				}
				pixel = (Uint8 *) (image->pixels + (y * width + x) * 4);
				if (--n<=0) break;
			}
		}
		else
		{
			// raw packet
			for (i=0; i<=b; i++)
			{
				fread(targa_pixel, bytes_per_pixel, 1, fp);
				pixel[0] = targa_pixel[2];
				pixel[1] = targa_pixel[1];
				pixel[2] = targa_pixel[0];
				if (depth == 32)
					pixel[3] = targa_pixel[3];
				else
					pixel[3] = 0xFF;
				if (origin == ORIGIN_BOTTOM_LEFT)
				{
					x++;
					if (x >= width)
					{
						x = 0;
						y--;
					}
				}
				else
				{
					x++;
					if (x >= width)
					{
						x = 0;
						y++;
					}
				}
				pixel = (Uint8 *) (image->pixels + (y * width + x) * 4);
				if (--n<=0) break;
			}
		}
	}
	fclose(fp);
	return image;
}

