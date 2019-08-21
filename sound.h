#include "SDL_mixer.h"

#ifndef SOUND_H
#define SOUND_H

typedef enum
{
        e_enginesound=0,
        e_firesound=1,
        e_explodesound=2,
        e_collectsound=3,
        e_alienvoice1=4,
        e_alienvoice2=5,
        e_alienvoice3=6
} channels;

int initsound(void);
void deinitsound(void);
int PlayWAV(channels,int);
int IsChannelPlaying(int);
void StopChannel(int);
void PlayOGG(char*);
void StopOGG(void);
#endif

