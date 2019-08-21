#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "SDL_mixer.h"
#include <unistd.h>

/* The screen surface */
SDL_Surface *screen = NULL;

void Terminate(void)
{
    //SaveScoreBoard();
    Shutdown();
	SDL_Quit();
//#ifdef GP2X
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
//#endif
}

int main (int argc, char *argv[])
{
	int done;

	/* Initialize SDL */
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		exit (1);
	}
	atexit (Terminate);
    SDL_JoystickOpen(0);
    
	/* Set 320x240 16-bits video mode */
	printf("SetVideoMode\n");
	screen = SDL_SetVideoMode (320, 240, 16, SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		fprintf (stderr, "Couldn't set 320x240x16 video mode: %s\n", SDL_GetError ());
		exit (2);
	}

	SDL_ShowCursor(SDL_DISABLE);

    done = 0;
    done=titlescreen();
    while(!done)
    {
        done=menu(0,NULL);
        if(done==0)
           rungame();
    }

    //save_high_scores();

    return 0;
}
