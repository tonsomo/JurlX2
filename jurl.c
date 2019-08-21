// *******************************************************
//
// NAME         : Jurl.c
// PROJECT      : Jurl
// DATE         : 2003-08-14 
// BY           : Jonn Blanchard
// 
// (c) Mission Integrated Systems
// 
// *******************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "jurl.h"
#include "highscore.h"
#include "fading.h"
#include "sound.h"

int score;
int gamedone;
int pelletcount;
int lives=3;

SCORE leaderboard[10];

void Shutdown()
{	
	SDL_FreeSurface(screen);																// Release the memory allocated to screen
    SDL_FreeSurface(PillGraphic);
    SDL_FreeSurface(font);
    SDL_FreeSurface(font_inv);
    SDL_FreeSurface(bulletimg);
    SDL_FreeSurface(playerimg);
    
    int i;
    
    for(i=0;i<NUM_ALIENTYPES;i++)
        SDL_FreeSurface(alienimg[i]);

    deinitsound();
}

void LoadImages()
{
    char filename[25];
    SDL_Surface* img_temp;
    int i;

    fprintf(stderr,"Load Images Start\n");

	if(!(img_temp=IMG_Load("data/gfx/power.png")))
	{
            fprintf(stderr,"Error: %s",SDL_GetError());
            exit(1);
    }

	PillGraphic = SDL_DisplayFormatAlpha(img_temp);
	SDL_FreeSurface(img_temp);

	if(!(img_temp=IMG_Load("data/gfx/bullet.png")))
	{
            fprintf(stderr,"Error: %s",SDL_GetError());
            exit(1);
    }
	
	bulletimg = SDL_DisplayFormatAlpha(img_temp);
	SDL_FreeSurface(img_temp);

	if(!(img_temp=IMG_Load("data/gfx/explode.png")))
	{
            fprintf(stderr,"Error: %s",SDL_GetError());
            exit(1);
    }
	
	explodeimg = SDL_DisplayFormatAlpha(img_temp);
	SDL_FreeSurface(img_temp);
	
    if(!(img_temp=IMG_Load("data/gfx/player.png")))
    {
            fprintf(stderr,"Error: %s",SDL_GetError());
            exit(1);
    }
    playerimg=SDL_DisplayFormatAlpha(img_temp);
    SDL_FreeSurface(img_temp);

 	for(i=0;i<NUM_ALIENTYPES;i++)
	{
            sprintf(filename,"data/gfx/alien%i.png",i+1);

            if(!(img_temp=IMG_Load(filename))) 
	        {
                    fprintf(stderr,"Error: %s",SDL_GetError());
                    exit(1);
            }
            alienimg[i]=SDL_DisplayFormatAlpha(img_temp);
            SDL_FreeSurface(img_temp);
    }
    fprintf(stderr,"Load Images End\n");
}

int vGetTickCount()
{
        return SDL_GetTicks();
}

int milliSleep(int timeout)
{
	SDL_Delay(timeout);
	return 1;
}

void fontOutlined(int left,int top,char* sztext,int fr, int fg, int fb)
{
     if(fr==100)
                Font_DrawText(screen,font_inv,sztext,left,top);
     else
                Font_DrawText(screen,font,sztext,left,top);
	//drawText(screen, sztext, left, top, fr, fg, fb);
}

int HTextExtent(const char* szText)
{
	int width;

	width=strlen(szText)*(font->w/256);
	return width;
}

int vTextExtent(const char* sztext)
{
	int height;

	height=font->h;
	return height;
}

int wait_for_start()
{
    return 0;
}

short int displayHighScores(SDL_Surface* output, SCORE* board, int wait)
{
	int i;
	SDL_Surface* img_temp;
	SDL_Surface* gamescreen;
	SDL_Event event;
	int done=0;

	if(!(img_temp=IMG_Load("data/gfx/starfld1.bmp")))
    	{
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    	}
    	gamescreen=SDL_DisplayFormat(img_temp);
    	SDL_FreeSurface(img_temp);

	while(!done)
	{
		drawSprite(gamescreen,output,0,0,0,0,gamescreen->w,gamescreen->h);

      	fontOutlined((output->w-HTextExtent("Leaderboard"))/2,5,"Leaderboard",255,255,255);
		int y=25;
		for(i=0;i<MAX_HISCORES;i++)
		{
			char outtext[20];
			sprintf(outtext,"%i. %s %i",i+1,board->name,board->score);
			fontOutlined((output->w-HTextExtent(outtext))/2,y,outtext,255,255,255);
			y+=vTextExtent(outtext);
			*board++;
		}

      	if(wait==1) fontOutlined((output->w-HTextExtent("- Start -"))/2,(output->h-vTextExtent("- Start -")),"-Start-",255,255,255);
		vFlipScreen(output);
		if(wait==1)
		{
    			while ( SDL_PollEvent(&event) ) {
  				if ( event.type == SDL_QUIT || (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == GP2X_BUTTON_START) )
    	 				done=1;
  			}
		} else
			done=1;
	}
	SDL_FreeSurface(gamescreen);
	return 0;
}

void enterHighScore(int score, int position)
{
	int i;
	char chrs[3];
	int pos=0;
	int done=0;
	int centY=0;
	int centX=0;
	SDL_Surface* img_temp;
	SDL_Surface* gamescreen;

	if(!(img_temp=IMG_Load("data/gfx/starfld1.bmp")))
    	{
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    	}
    	gamescreen=SDL_DisplayFormatAlpha(img_temp);
    	SDL_FreeSurface(img_temp);

	centY=(screen->h-vTextExtent("A A A"))/2;
	centX=(screen->w-HTextExtent("A A A"))/2;

	chrs[0]=65; chrs[1]=65; chrs[2]=65;

	for(i=MAX_HISCORES-1;i>position;i--)
	{
		sprintf(leaderboard[i].name,"%s",leaderboard[i-1].name);
		leaderboard[i].score=leaderboard[i-1].score;
	}
	
	vClearScreen(0);
	vFlipScreen(screen);
	vClearScreen(0);
	while(!done)
	{
		drawSprite(gamescreen,screen,0,0,0,0,gamescreen->w,gamescreen->h);
      	fontOutlined((screen->w-HTextExtent("Enter Highscore"))/2,5,"Enter Highscore",255,255,255);	
		SDL_Event event;
    		while ( SDL_PollEvent(&event) ) 
		{
  			if (event.type == SDL_JOYBUTTONDOWN || event.type == SDL_KEYDOWN)
			{
				if(event.jbutton.button == GP2X_BUTTON_START || event.key.keysym.sym == SDLK_SPACE)
    	 				done=1;
				else if(event.jbutton.button == GP2X_BUTTON_B || event.key.keysym.sym == SDLK_RIGHT)
				{
					pos++;
					if(pos==3)
					{
						pos=2;
						done=1;
					}
				} else if(event.jbutton.button == GP2X_BUTTON_A || event.key.keysym.sym == SDLK_LEFT)
				{
					pos--;
					if(pos<0) pos=0;
				} else if(event.jbutton.button == GP2X_BUTTON_UP || event.key.keysym.sym == SDLK_UP)
				{
					chrs[pos]++;
					if(chrs[pos]==93)
						chrs[pos]=32;
					else if(chrs[pos]==33)
						chrs[pos]=65;
				} else if(event.jbutton.button == GP2X_BUTTON_DOWN || event.key.keysym.sym == SDLK_DOWN)
				{
					chrs[pos]--;
					if(chrs[pos]==64)
						chrs[pos]=32;
					else if(chrs[pos]==31)
						chrs[pos]=92;
  				}
			}
			
		}

		for(i=0;i<3;i++)
		{
			int x=0;
			char tmp[1];				

			sprintf(tmp,"%c",chrs[i]);
		
			if(i==0)
				x=centX-HTextExtent("A ");
			else if(i==1)
				x=centX;
			else if(i==2)
				x=centX+HTextExtent(" A");

			if(pos==i)
				fontOutlined(x,centY,tmp,100,100,100);
			else
				fontOutlined(x,centY,tmp,255,255,255);
		}
		vFlipScreen(screen);
	}
	sprintf(leaderboard[position].name,"%c%c%c",chrs[0],chrs[1],chrs[2]);
	leaderboard[position].score=score;
	saveHiScores(leaderboard);

	SDL_FreeSurface(gamescreen);
}

short int checkHighScore(int score)
{
	int i,found=-1;
	for(i=0;i<MAX_HISCORES;i++)
	{
		if(leaderboard[i].score<score)
		{
			found=i;
			break;
		}
	}

	if(found>-1)
	{
		enterHighScore(score,found);
		displayHighScores(screen,leaderboard,1);
		
		return 1;
	}

	return 0;
}

void vSetRandom(int seed)
{
	srand(seed);
}

int vGetRandom(int toprange)
{
	return (rand () % toprange) + 1;
}

void vClearScreen(int colour)
{
	SDL_FillRect(screen, NULL, colour);
}

void vFlipScreen(SDL_Surface *flip)
{
	SDL_Flip(flip);
}

void displayCredits(SDL_Surface *disp)
{
     SDL_Surface* creds = IMG_Load("data/gfx/credits.png");
     
     drawSprite(creds,disp,0,0,0,0,creds->w,creds->h);
     vFlipScreen(disp);
     
     SDL_FreeSurface(creds);
}

int menu(int menutype,SDL_Surface* backdrop)
{
	int option=-1;
	int choice=0;
	SDL_Event keys;
	int lastchange=0;
    	Uint8* keystate;
    	SDL_Surface* men_play_nosel=NULL;
    	SDL_Surface* men_play_sel=NULL;
    	SDL_Surface* men_quit_nosel=NULL;
    	SDL_Surface* men_quit_sel=NULL;
    	SDL_Surface* MenuBMP=NULL;
	int flipshow=vGetTickCount()+8000;
	int showtype=0;  
    Uint32 now_time, last_time;

    switch(menutype)
    {
         case 0:
              if(!(men_play_nosel = IMG_Load("data/gfx/men_play_nosel.png")))
                   fprintf(stderr,"Error: %s",SDL_GetError());

              if(!(men_play_sel = IMG_Load("data/gfx/men_play_sel.png")))
                   fprintf(stderr,"Error: %s",SDL_GetError());
              if(!(men_quit_nosel = IMG_Load("data/gfx/men_quit_nosel.png")))
                   fprintf(stderr,"Error: %s",SDL_GetError());
              if(!(men_quit_sel = IMG_Load("data/gfx/men_quit_sel.png")))
                   fprintf(stderr,"Error: %s",SDL_GetError());
              break;
         case 1:
              men_play_nosel = IMG_Load("data/gfx/men_resume_nosel.png");
              men_play_sel = IMG_Load("data/gfx/men_resume_sel.png");
              men_quit_nosel = IMG_Load("data/gfx/men_quit_nosel.png");
              men_quit_sel = IMG_Load("data/gfx/men_quit_sel.png");
              break;
         case 2:
              men_play_nosel = IMG_Load("data/gfx/men_yes_nosel.png");
              men_play_sel = IMG_Load("data/gfx/men_yes_sel.png");
              men_quit_nosel = IMG_Load("data/gfx/men_no_nosel.png");
              men_quit_sel = IMG_Load("data/gfx/men_no_sel.png");
              break;
    }

	vClearScreen(0);

    if(menutype==0)
    {
         MenuBMP = IMG_Load("data/gfx/menu.bmp");
         fade (screen, MenuBMP, 2, 0) ;
    }

	while(option==-1)
	{
             last_time = SDL_GetTicks();

    		 SDL_PollEvent(&keys);
		     if ( keys.type == SDL_QUIT || (keys.type == SDL_JOYBUTTONDOWN && keys.jbutton.button == GP2X_BUTTON_SELECT) )
             {
	            return 0;
             }

             if(keys.type == SDL_JOYBUTTONDOWN || keys.type == SDL_KEYDOWN)
             {
                  showtype=0;
		          flipshow=vGetTickCount()+8000;
             }

	  if(menutype==0 && showtype==1)
	  {
            drawSprite(MenuBMP,screen,0,0,0,0,MenuBMP->w,MenuBMP->h);
		    displayHighScores(screen,leaderboard,0);
      } else if(menutype==0 && showtype==2)
      {
            displayCredits(screen);
	  } else
	  {
        if(menutype==0)
            drawSprite(MenuBMP,screen,0,0,0,0,MenuBMP->w,MenuBMP->h);
        else
            drawSprite(backdrop,screen,0,0,0,0,backdrop->w,backdrop->h);
            
		//if(lastchange==0||vGetTickCount()-lastchange>100)
		//{
             keystate=SDL_GetKeyState(NULL);
             //if(keystate[SDLK_ESCAPE] == SDL_PRESSED)
             //   return 0;
             if(keystate[SDLK_UP] == SDL_PRESSED || (keys.type == SDL_JOYBUTTONDOWN && keys.jbutton.button == GP2X_BUTTON_UP) )
                {
                choice-=1;
                }
             else if(keystate[SDLK_DOWN] == SDL_PRESSED || (keys.type == SDL_JOYBUTTONDOWN && keys.jbutton.button == GP2X_BUTTON_DOWN) )
                choice+=1;
             else if(keystate[SDLK_SPACE] == SDL_PRESSED || (keys.type == SDL_JOYBUTTONDOWN && keys.jbutton.button == GP2X_BUTTON_X) )
                option=choice;
                
             lastchange=vGetTickCount();
        //}

		if(choice>1) choice=0;
		if(choice<0) choice=1;

		int pos=150;

		if(choice==0)
        {
             drawSprite(men_play_sel,screen,0,0,(screen->w-men_play_sel->w)/2,pos,men_play_sel->w,men_play_sel->h);
             pos+=men_play_sel->h;
        } else
        {
             drawSprite(men_play_nosel,screen,0,0,(screen->w-men_play_nosel->w)/2,pos,men_play_nosel->w,men_play_nosel->h);
             pos+=men_play_nosel->h;
        }

		if(choice==1)
        {
             drawSprite(men_quit_sel,screen,0,0,(screen->w-men_quit_sel->w)/2,pos,men_quit_sel->w,men_quit_sel->h);
             pos+=men_quit_sel->h;
        } else
        {
             drawSprite(men_quit_nosel,screen,0,0,(screen->w-men_quit_nosel->w)/2,pos,men_quit_nosel->w,men_quit_nosel->h);
             pos+=men_quit_nosel->h;
        }

		vFlipScreen(screen);
	  }
	  if(vGetTickCount()-flipshow>=0)
	  {
		if(showtype==0)
			showtype=1;
		else if(showtype==1)
			showtype=2;
		else
		    showtype=0;

		flipshow=vGetTickCount()+8000;
	  }

          now_time = SDL_GetTicks();
      
          if (now_time < last_time + (2000 / FPS))
	      {
	          SDL_Delay(last_time + 2000 / FPS - now_time);
          }
	}
	//if(option==1)
	//	return 1;
	//else
	//	return 0;

    if(menutype==0) SDL_FreeSurface(MenuBMP);
    SDL_FreeSurface(men_play_nosel);
    SDL_FreeSurface(men_play_sel);
    SDL_FreeSurface(men_quit_nosel);
    SDL_FreeSurface(men_quit_sel);

    StopOGG();
    return option;
}

int titlescreen()
{
    Uint8* keystate;
    SDL_Event event;
    SDL_Surface* temp_img;

    SDL_Surface* TitleBMP1;
    SDL_Surface* TitleBMP2;

    LoadFont(12);
    LoadImages();
    initsound();
    printf("Start: load_high_scores\n");
    if(!(loadHiScores(leaderboard)))
    {
	  if(!(createHiScoreTable()))
		fprintf(stderr,"Error saving High Score Table\n");
      else
          loadHiScores(leaderboard);
    }

    printf("Finish: load_high_scores\n");

    if(!(temp_img = IMG_Load("data/gfx/Intro1.bmp")))
    {
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    }
    TitleBMP1=SDL_DisplayFormat(temp_img);
    SDL_FreeSurface(temp_img);    
    
    if(!(temp_img = IMG_Load("data/gfx/Intro2.bmp")))
    {
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    }
    TitleBMP2=SDL_DisplayFormat(temp_img);
    SDL_FreeSurface(temp_img);    
    
    fade (screen, TitleBMP1, 2, 0) ;
    SDL_Delay (2000) ;
    crossFade (screen, TitleBMP1, TitleBMP2, 2) ;
    
    int done=-1;
    int pp=0;
    int lastchange=0;
    
    PlayOGG("data/music/spar.ogg");
    while(done==-1)
    {
        drawSprite(TitleBMP2,screen,0,0,0,0,TitleBMP2->w,TitleBMP2->h);
    	while ( SDL_PollEvent(&event) ) {
  			if ( event.type == SDL_QUIT || (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == GP2X_BUTTON_START) )
    	 			done=0;
  		}				

        keystate=SDL_GetKeyState(NULL);
        if(keystate[SDLK_ESCAPE] == SDL_PRESSED)
            done=1;
        else if(keystate[SDLK_SPACE] == SDL_PRESSED)
            done=0;
            
        if(lastchange==0 || vGetTickCount()-lastchange>1000)
        {
            if(pp==0)
                 pp=1;
            else
                 pp=0;

            lastchange=vGetTickCount();
        }

        if(pp==0) fontOutlined((screen->w-HTextExtent("- Start -"))/2,((screen->h-vTextExtent("- Start -"))/2)+10,"-Start-",255,255,255);
        vFlipScreen(screen);
    }
    fade (screen, TitleBMP2, 2, 1) ;

    SDL_FreeSurface(TitleBMP1);
    SDL_FreeSurface(TitleBMP2);
      
    return done;
}

void deinit()
{
}

void drawPlayer(int x, int y, int angle, int up_pressed)
{
    if (player.alive)
    {
       //drawSprite(playerimg[angle],screen,0,0,x,y,24,24);
       if(player.IsExplosion==1)
            drawSprite(explodeimg,screen,24*player.animstep,0,x,y,24,24);
       else
            drawSprite(playerimg,screen,24*angle,0,x,y,24,24);
            
       if(player.animcount>0)
       {
           if(vGetTickCount()-player.nexttime>=0)
           {
               player.animstep++;
               if(player.animstep>=player.animcount)
                   ResetGame();
               
               player.nexttime=vGetTickCount()+130;
           }
       }
    }
}
    
void movebullets()
{
     int i;
     
     for (i = 0; i < MAXPLAYERBULLETS+MAXALIENBULLETS; i++)
	 {
         if (bullets[i].timer >= 0)
	     {
            /* Bullet wears out: */
	        //bullets[i].timer--;
      
	        /* Move bullet: */    
	        bullets[i].x = bullets[i].x + bullets[i].xdir;
	        bullets[i].y = bullets[i].y + bullets[i].ydir;
	      
	        /* Wrap bullet around edges of screen: */
	        if (bullets[i].x >= WINDOW_WIDTH)
		       bullets[i].timer=-1;
            else if (bullets[i].x < 0)
		       bullets[i].timer=-1;
	      
            if (bullets[i].y >= WINDOW_HEIGHT)
		       bullets[i].timer=-1;
	        else if (bullets[i].y < 0)
		       bullets[i].timer=-1;
         }
	}
}

void drawbullets()
{
     int i;
     
     for(i=0;i < MAXPLAYERBULLETS+MAXALIENBULLETS; i++)
     {
           if(bullets[i].timer >= 0)
           {
                 drawSprite(bulletimg,screen,0,0,bullets[i].x,bullets[i].y,bulletimg->w,bulletimg->h);
           }
     }
}

void drawPellets()
{
     int i;
     
     for(i=0;i<26;i++)
     {
           if(pellets[i].alive==1)
                 drawSprite(PillGraphic,screen,0,0,pellets[i].x,pellets[i].y,PillGraphic->w,PillGraphic->h);
     }
}

void doGetPellet(int pellet)
{
     pellets[pellet].alive=0;
     PlayWAV(3,2);
     score+=10;
     pelletcount-=1;
     if(pelletcount==0)
            gamedone=1;
}

void drawAliens()
{
     int i;
     
     for(i=0;i<MAXALIENS;i++)
          if(aliens[i].alive==1)
          {
             if(aliens[i].IsExplosion==0)
             {
                 drawSprite(alienimg[aliens[i].object],screen,0,0,aliens[i].x,aliens[i].y,alienimg[aliens[i].object]->w,alienimg[aliens[i].object]->h);
             }
             else
             {
                 drawSprite(explodeimg,screen,24*aliens[i].animstep,0,aliens[i].x,aliens[i].y,24,24);
             }
          }
}

void loseLife()
{
    if(player.IsExplosion==1) return;
    StopChannel(1);
    PlayWAV(2,1);
    player.nexttime=vGetTickCount()+30;
    player.IsExplosion=1;
    player.animcount=5;
    player.animstep=0;
    lives--;     
}

void hitAlien(int alien)
{
     if(aliens[alien].alive==0 || aliens[alien].IsExplosion==1) return;
     aliens[alien].lives--;
     if(aliens[alien].lives<=0)
     {
         PlayWAV(2,3);
         aliens[alien].IsExplosion=1;
         aliens[alien].animcount=5;
         aliens[alien].animstep=0;
     }         
}

void collisions()
{
     int i;
     SDL_Surface* playimg = GetSpriteImage(playerimg,24*player.angle,0,24,24);
     
     // Pellets
     for(i=0;i<26;i++)
            //if(pellets[i].alive==1 && player.x>=pellets[i].x && player.x<=pellets[i].x+PillGraphic->w && player.y>=pellets[i].y && player.y<=pellets[i].y+PillGraphic->h)
            if(pellets[i].alive==1)
            {

                 if(collision_detect(pellets[i].x,pellets[i].y,player.x,player.y,PillGraphic,playimg)==1)
                 {
                       doGetPellet(i);
                       break;
                 }      
            }
            
      // Bullets
      for(i=0;i<MAXPLAYERBULLETS+MAXALIENBULLETS;i++)
             if(bullets[i].timer>=0)
             {
                  if(bullets[i].owner==-1)
                  {
                        int i2;
                        
                        for(i2=0;i2<MAXALIENS;i2++)
                        {
                               if(aliens[i2].alive==1 && aliens[i2].object>-1)
                                   if(collision_detect(bullets[i].x,bullets[i].y,aliens[i2].x,aliens[i2].y,bulletimg,alienimg[aliens[i2].object])==1)
                                   {
                                        score+=10*(aliens[i2].object+1);
                                        hitAlien(i2);
                                        bullets[i].timer=-1;
                                        break;
                                   }
                        }
                  } else
                  {
                        if(collision_detect(bullets[i].x,bullets[i].y,player.x,player.y,bulletimg,playimg)==1)
                        {
                               bullets[i].timer=-1;
                               loseLife();
                               break;
                        }       
                  }
             }  
      // Aliens
      for(i=0;i<MAXALIENS;i++)
          if(aliens[i].IsExplosion==0 && aliens[i].alive==1 && player.IsExplosion==0)
             if(collision_detect(aliens[i].x,aliens[i].y,player.x,player.y,alienimg[aliens[i].object],playimg)==1)
             {
                   //aliens[i].object=-1;                                                                                                                                                                                                                                                                    
                   loseLife();
                   break;
             }

      SDL_FreeSurface(playimg);
}

int GetObjectBullets(int object)
{
    int i;
    int count=0;
    
    for(i=0;i<MAXPLAYERBULLETS+MAXALIENBULLETS;i++)
    {
          if(bullets[i].owner==object && bullets[i].timer>=0)
              count++;
    }                                   

    return count;
}

void LoadPellets()
{
     int i,x,y;
     int xs,ys;
     xs=(screen->w-10)/5;
     ys=(screen->h-25)/5;

     x=xs/2;y=(ys/2)+(ys/4);

     for(i=0;i<26;i++)
     {
          pellets[i].x=x;
          pellets[i].y=y;
          pellets[i].alive=1;
          pellets[i].xdir=0;
          pellets[i].ydir=0;
          
          x+=xs;
          if(i>0 && i%5==0)
          {
                x=xs/2;
                y+=ys;
          }
     }
     
     pelletcount=25;
}

void movealiens()
{
     int i;
     int angle=0,randchoice=0;
     int xdiff=0,ydiff=0;
     veloc_type mod;
     
     for(i=0;i<MAXALIENS;i++)
     {
         if(aliens[i].alive==1 && vGetTickCount()-aliens[i].nexttime>=0)
         {
             if(aliens[i].IsExplosion==0)
                 switch(aliens[i].object)
                 {
                     case 0: // dumb
                          if(vGetRandom(10)<5)
                          {
                              aliens[i].angle=vGetRandom(15);
                              mod=CalcVelocity(aliens[i].angle);
                              aliens[i].xdir=mod.xmod;
                              aliens[i].ydir=mod.ymod;
                          }
                          
                          break;
                     case 1: // seeker
                          if(player.x<aliens[i].x) aliens[i].xdir=-1;
                          if(player.x>aliens[i].x) aliens[i].xdir=1;
                          if(player.y<aliens[i].y) aliens[i].ydir=-1;
                          if(player.y>aliens[i].y) aliens[i].ydir=1;
                          break;
                     case 2: // shooter
                          randchoice=vGetRandom(15);
                          
                          if(randchoice>10 && GetObjectBullets(i)>0)
                                randchoice=5;
                          
                          if(randchoice<5) // Just mill around
                          {
                              aliens[i].angle=vGetRandom(15);
                              mod=CalcVelocity(aliens[i].angle);
                              aliens[i].xdir=mod.xmod;
                              aliens[i].ydir=mod.ymod;
                          } else if(randchoice<=10) // Seek
                          {
                              if(player.x<aliens[i].x) aliens[i].xdir=-1;
                              if(player.x>aliens[i].x) aliens[i].xdir=1;
                              if(player.y<aliens[i].y) aliens[i].ydir=-1;
                              if(player.y>aliens[i].y) aliens[i].ydir=1;
                          } else if(randchoice>10) // Shoot
                          {
                              // Steps
                              // 1. Find angle to player
                              // 2. add_bullet                             
                              xdiff = player.x-aliens[i].x;
                              ydiff = player.y-aliens[i].y;
                              if(xdiff==0)
                              {
                                  if(ydiff<0)
                                     angle=0;
                                  else
                                     angle=8;
                              } else if(xdiff<0)
                              {
                                   if(ydiff<0)
                                      angle=14;
                                   else if(ydiff>0)
                                      angle=10;
                                   else
                                      angle=12;
                              } else if(xdiff>0)
                              {
                                   if(ydiff<0)
                                      angle=2;
                                   else if(ydiff>0)
                                      angle=6;
                                   else
                                      angle=4;
                              }
                              mod=CalcVelocity(angle);
                              mod.xmod=mod.xmod*2;
                              mod.ymod=mod.ymod*2;
                              PlayWAV(1,5);
                              add_bullet(aliens[i].x+((24-bulletimg->w)/2),aliens[i].y+((24-bulletimg->h)/2),angle,mod.xmod,mod.ymod,i);
                          }
                          break;
                 }
                 
                 if(aliens[i].object>-1)
                 {
                      aliens[i].x+=aliens[i].xdir;
                      aliens[i].y+=aliens[i].ydir;
                 
                      if(aliens[i].y<20) aliens[i].y=screen->h-alienimg[aliens[i].object]->h;
                      if(aliens[i].y>screen->h) aliens[i].y=20;
                      if(aliens[i].x<0) aliens[i].x=screen->w-alienimg[aliens[i].object]->w;
                      if(aliens[i].x>screen->w) aliens[i].x=0;
                 }
             
             if(aliens[i].animcount>0) aliens[i].animstep++;
             if(aliens[i].animstep>aliens[i].animcount)
             {
                 if(aliens[i].IsExplosion==1)
                 {
                     aliens[i].alive=0;
                 }
                 else
                 {
                     aliens[i].animcount=0;
                 }
             }
             aliens[i].nexttime=vGetTickCount()+20;
         }
     }
}

int rungame(void)
{
    SDL_Surface* gamescreen;
    SDL_Surface* starfield;
    SDL_Surface* levelcomplete;
    SDL_Surface* gameover;
    SDL_Surface* img_temp;
    SDL_Event event;
    Uint8* keystate;
    int accel=0;
    int lastchange=0;
    int lastchange_mov=0;
    int done=-1;
    int enginechannel=-1;
    Uint32 now_time,last_time;
    
    vClearScreen(0);
    
    if(!(img_temp=IMG_Load("data/gfx/game.png")))
    {
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    }
    gamescreen=SDL_DisplayFormatAlpha(img_temp);
    SDL_FreeSurface(img_temp);
    
    if(!(img_temp=IMG_Load("data/gfx/starfld1.bmp")))
    {
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    }
    starfield=SDL_DisplayFormat(img_temp);
    SDL_FreeSurface(img_temp);

    if(!(img_temp=IMG_Load("data/gfx/levelcomplete.png")))
    {
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    }
    levelcomplete=SDL_DisplayFormat(img_temp);
    SDL_FreeSurface(img_temp);

    if(!(img_temp=IMG_Load("data/gfx/gameover.png")))
    {
         fprintf(stderr,"Error: %s",SDL_GetError());
         exit(1);
    }
    gameover=SDL_DisplayFormatAlpha(img_temp);
    SDL_FreeSurface(img_temp);

    SDL_PollEvent(&event);
    keystate=SDL_GetKeyState(NULL);
    
    score=0;
    done=-1;
    lives=3;
    level=1;
    while(done==-1)
    {
        gamedone=0;
        LoadPellets();
        vSetRandom(1337);
        ResetGame();
        while(done==-1 && gamedone==0)
        {
            last_time=vGetTickCount();
            
        	SDL_PollEvent(&event);

            keystate=SDL_GetKeyState(NULL);
            if(keystate[SDLK_ESCAPE] == SDL_PRESSED || (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == GP2X_BUTTON_START))
            {
                SDL_Surface* ImgBackDrop = SDL_DisplayFormat(screen);
                drawSprite(screen,ImgBackDrop,0,0,0,0,screen->w,screen->h);
                done=menu(1,ImgBackDrop);
                if(done==0) done=-1;
                SDL_FreeSurface(ImgBackDrop);
            }
            
            if(done<1 && player.IsExplosion==0)
            {
                    if(keystate[SDLK_RIGHT] == SDL_PRESSED || (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == GP2X_BUTTON_RIGHT))
                    {
                           player.angle+=1;
                           if(player.angle>15) player.angle=0;
                    }
                    if(keystate[SDLK_LEFT] == SDL_PRESSED || (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == GP2X_BUTTON_LEFT))
                    {
                           player.angle-=1;
                           if(player.angle<0) player.angle=15;
                    }
                    if(keystate[SDLK_UP] == SDL_PRESSED || (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == GP2X_BUTTON_UP))
                    {
                           int xmod,ymod;
                           veloc_type mod;
                           
                           mod=CalcVelocity(player.angle);
                           xmod=mod.xmod;
                           ymod=mod.ymod;                                          
    
                           accel=1;
    	                   player.xdir = player.xdir + xmod;
    	                   player.ydir = player.ydir + ymod;
                    } else
                           accel=0;
                           
                    if(keystate[SDLK_DOWN] == SDL_PRESSED || (event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == GP2X_BUTTON_B || event.jbutton.button == GP2X_BUTTON_DOWN)))
                    {
                           veloc_type mod;
                           int oldxd = player.xdir;
                           int oldyd = player.ydir;
                         
                           mod=CalcVelocity(player.angle);
                           //accel=1;
                           if(player.xdir!=0) player.xdir = player.xdir + 0-(player.xdir/1.5); //(player.xdir * 10) / 8;
                           if(player.ydir!=0) player.ydir = player.ydir + 0-(player.ydir/1.5);//(player.ydir * 10) / 8;
                           
                           if(oldxd<0 && player.xdir>0) player.xdir=0;
                           if(oldxd>0 && player.xdir<0) player.xdir=0;
                           if(oldyd<0 && player.ydir>0) player.ydir=0;
                           if(oldyd>0 && player.ydir<0) player.ydir=0;
                    }
                    
                    if(keystate[SDLK_SPACE] == SDL_PRESSED || (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == GP2X_BUTTON_X))
                    {
                           if(vGetTickCount()-player.nexttime>=0)
                           {
                                 veloc_type mod;
                                 mod=CalcVelocity(player.angle);
                                 mod.xmod=mod.xmod*3;
                                 mod.ymod=mod.ymod*3;
                           
                                 PlayWAV(1,6);
    		                     add_bullet(player.x+((24-bulletimg->w)/2), player.y+((24-bulletimg->h)/2), player.angle, mod.xmod, mod.ymod,-1);
    		                     player.nexttime=vGetTickCount()+100;
                           }
                    }
    		        
    		//        lastchange=vGetTickCount();
            }
        
            if(player.IsExplosion==0)
            {
                    // Movement
                    if(player.xdir>4) player.xdir=4;
                    if(player.xdir<-4) player.xdir=-4;
                    if(player.ydir>4) player.ydir=4;
                    if(player.ydir<-4) player.ydir=-4;
    
                    player.x=player.x+player.xdir;
                    player.y=player.y+player.ydir;
    
                    if(player.x>screen->w) player.x=0;
                    if(player.y>screen->h) player.y=10;
                    if(player.x<0) player.x=screen->w;
                    if(player.y<10) player.y=screen->h;
    
                    movebullets();
                    movealiens();
                    
            //        lastchange_mov=vGetTickCount();
            }

            if(accel==1)
                if(IsChannelPlaying(CHAN_THRUST)==0) PlayWAV(CHAN_THRUST,0);
            else
                StopChannel(CHAN_THRUST);

            collisions();
            // Render Cycle
            
            // Starfield
            drawSprite(starfield,screen,0,0,0,0,starfield->w,starfield->h);
            drawPellets();
            drawbullets();
            drawAliens();
            drawPlayer(player.x,player.y,player.angle,accel);      
    
            // Always draw gamescreen last        
            drawSprite(gamescreen,screen,0,0,0,0,gamescreen->w,gamescreen->h);
            
            char str_score[6+9];
            char str_lives[6+4];
            
            sprintf(str_score,"Score %i",score);
            sprintf(str_lives,"Lives %i",lives);      
            
            fontOutlined(69,5,str_score,255,100,100);
            fontOutlined(245,5,str_lives,255,100,100);        
    
            vFlipScreen(screen);
            if(lives<=0) done=1;

            now_time = SDL_GetTicks();
      
            if (now_time < last_time + (1000 / FPS))
	        {
	            SDL_Delay(last_time + 1000 / FPS - now_time);
            }
        }
        if(lives>0 && pelletcount==0) // won
        {
            fade (screen, levelcomplete, 2, 0) ;
            level++;
            //drawSprite(levelcomplete,screen,0,0,(screen->w-levelcomplete->w)/2,(screen->h-levelcomplete->h)/2,levelcomplete->w,levelcomplete->h);
            //vFlipScreen(screen);
            SDL_Delay(1000);
        }        
        if(lives<=0) // lost
        {
            fade (screen, gameover,2,0);
            SDL_Delay(1000);
            done=0;
        }
    }
    SDL_FreeSurface(gamescreen);
    SDL_FreeSurface(starfield);
    SDL_FreeSurface(levelcomplete);
    SDL_FreeSurface(gameover);

    checkHighScore(score);
    
    return 0;
}

void FireShot(int AlienIndex, int AlienX, int AlienY,int ShipX,int ShipY)
{

}
