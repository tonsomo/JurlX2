#include "sound.h"

Mix_Chunk *enginesound = NULL;
Mix_Chunk *firesound = NULL;
Mix_Chunk *explodesound = NULL;
Mix_Chunk *collectsound = NULL;
Mix_Chunk *alienvoice1 = NULL;
Mix_Chunk *alienvoice2 = NULL;
Mix_Chunk *alienvoice3 = NULL;
Mix_Music *music = NULL; 

int initsound()
{
    Mix_OpenAudio(22050, AUDIO_S16, 2, 256);			// Initialize SDL_mixer

	enginesound = Mix_LoadWAV("data/sfx/thrust.wav");
    firesound= Mix_LoadWAV("data/sfx/blowback.wav");
    explodesound= Mix_LoadWAV("data/sfx/explosion.wav");
    collectsound= Mix_LoadWAV("data/sfx/collect.wav");
    
    return 1;
}

void deinitsound()
{	
	Mix_FreeChunk(enginesound);																		// Release the memory allocated to sound
	Mix_FreeChunk(firesound);																		// Release the memory allocated to sound
	Mix_FreeChunk(explodesound);																		// Release the memory allocated to sound
    Mix_FreeChunk(collectsound);
    Mix_FreeChunk(alienvoice1);
    Mix_FreeChunk(alienvoice2);
    Mix_FreeChunk(alienvoice3);
	Mix_FreeMusic(music);																		// Release the memory allocated to music
	Mix_CloseAudio();																				// Close SDL_Mixer Audio
}

int PlayWAV(channels sound,int chan)
{
	int channel=0;

    switch(sound)
    {
        case 0:
	         if(enginesound) channel = Mix_PlayChannel(chan, enginesound, 0);
             break;
        case 1:
	         if(firesound) channel = Mix_PlayChannel(chan, firesound, 0);
	         break;
        case 2:
	         if(explodesound) channel = Mix_PlayChannel(chan, explodesound, 0);
	         break;
        case 3:
	         if(collectsound) channel = Mix_PlayChannel(chan, collectsound, 0);										
	         break;
        case 4:
	         if(alienvoice1) channel = Mix_PlayChannel(chan, alienvoice1, 0);
	         break;
        case 5:
	         if(alienvoice2) channel = Mix_PlayChannel(chan, alienvoice2, 0);
	         break;
        case 6:
	         if(alienvoice3) channel = Mix_PlayChannel(chan, alienvoice3, 0);
	         break;
    }

    return channel;
}
int IsChannelPlaying(int channel)
{
    return Mix_Playing(channel);
}

void StopChannel(int channel)
{
    Mix_HaltChannel(channel);
}

void PlayOGG(char* filename)
{
	//SDL_Mixer OGG play test

    printf("Loaded %s\n",filename);
	if(!(music = Mix_LoadMUS(filename)))										// Load music.ogg from sdcard
        fprintf(stderr,"Error: %s",SDL_GetError());
	Mix_PlayMusic(music, 1);														// Play the music
}

void StopOGG()
{
     Mix_HaltMusic();
     music=NULL;
}
