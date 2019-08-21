#include "font.h"

#ifndef __JURL_H
#define __JURL_H

#define GP2X_BUTTON_UP              (0)
#define GP2X_BUTTON_DOWN            (4)
#define GP2X_BUTTON_LEFT            (2)
#define GP2X_BUTTON_RIGHT           (6)
#define GP2X_BUTTON_UPLEFT          (1)
#define GP2X_BUTTON_UPRIGHT         (7)
#define GP2X_BUTTON_DOWNLEFT        (3)
#define GP2X_BUTTON_DOWNRIGHT       (5)
#define GP2X_BUTTON_CLICK           (18)
#define GP2X_BUTTON_A               (12)
#define GP2X_BUTTON_B               (13)
#define GP2X_BUTTON_Y               (14)
#define GP2X_BUTTON_X               (15)
#define GP2X_BUTTON_L               (10)
#define GP2X_BUTTON_R               (11)
#define GP2X_BUTTON_START           (8)
#define GP2X_BUTTON_SELECT          (9)
#define GP2X_BUTTON_VOLUP           (16)
#define GP2X_BUTTON_VOLDOWN         (17)

typedef unsigned char u8;

const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 240;
const char* WINDOW_TITLE = "Jurl";
SDL_Surface* screen;
SDL_Surface* font;
SDL_Surface* font_inv;
SDL_Surface* playerimg;
SDL_Surface* bulletimg;
SDL_Surface* explodeimg;
int level=1;

typedef struct
{
	int	x,y;
	int	xdir,ydir;
	int object;
	int alive;
	int owner;
	int nexttime;
	int lasttime;
	int angle;
	int lives;
	int timer;
	int IsExplosion;
	int animcount;
	int animstep;
}	posNdir_t;

typedef struct veloc_type {
  int xmod;
  int ymod;
} veloc_type;

// MAX Objects
#define NROBJECTS 40
#define MAXALIENS 10
#define MAXPLAYERBULLETS 5
#define MAXALIENBULLETS 1
#define MAXPILLS 25
#define PLAYERFRAMETIME 100
#define ALIENFRAMETIME 1000
#define SHIELDMAX 3
#define LEFT_EDGE   0x0001
#define RIGHT_EDGE  0x0002
#define TOP_EDGE    0x0004
#define BOTTOM_EDGE 0x0008
#define AST_SIDES 6
#define AST_RADIUS 7
#define SHIP_RADIUS 12
#define NUM_ALIENTYPES 3
#define FPS 50
#define CHAN_THRUST 0

SDL_Surface* PillGraphic;
SDL_Surface* alienimg[NUM_ALIENTYPES+1];

posNdir_t player;
posNdir_t pellets[26];
posNdir_t aliens[MAXALIENS+1];

posNdir_t bullets[MAXPLAYERBULLETS+MAXALIENBULLETS];

// Pointer to the joystick interface
SDL_Joystick *gJoystick = NULL;

// Functions
int menu(int,SDL_Surface*);
void FireShot(int, int, int,int,int);
int rungame(void);
void LoadImages(void);
int vGetTickCount(void);
int milliSleep(int);
void fontOutlined(int,int,char*,int,int,int);
int vTextExtent(const char*);
int HTextExtent(const char*);
void vSetRandom(int);
int vGetRandom(int);
void vClearScreen(int);
SDL_Surface *GetSpriteImage(SDL_Surface*,int x, int y, int w, int h);
void vFlipScreen(SDL_Surface*);
veloc_type CalcVelocity(int angle);
short int collision_detect(int x1,int y1,int x2,int y2,SDL_Surface* img1, SDL_Surface* img2);
void drawSprite(SDL_Surface*, SDL_Surface*, int, int, int, int, int, int);
void ResetScoreboard();
void SaveScoreBoard();
Uint32 getpixel(SDL_Surface *surface, int x, int y);

Uint32 getpixel(SDL_Surface *surface, int x, int y) {
  
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    return *p;

  case 2:
    return *(Uint16 *)p;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      return p[0] << 16 | p[1] << 8 | p[2];
    else
      return p[0] | p[1] << 8 | p[2] << 16;

  case 4:
    return *(Uint32 *)p;

  default:
    return 0;       /* shouldn't happen, but avoids warnings */
  }  
}

SDL_Surface *GetSpriteImage(SDL_Surface *Image,int x, int y, int w, int h)
{
     Uint32 rmask, gmask, bmask, amask;
     SDL_Surface * destSurface = NULL;
     #if SDL_BYTEORDER == SDL_BIG_ENDIAN
         rmask = 0xff000000;
         gmask = 0x00ff0000;
         bmask = 0x0000ff00;
         amask = 0x000000ff;
     #else
          rmask = 0x000000ff;
          gmask = 0x0000ff00;
          bmask = 0x00ff0000;
          amask = 0xff000000;
     #endif
     x=0; y=0;

     destSurface = SDL_CreateRGBSurface(Image->flags, w, h, 32, rmask, gmask, bmask, amask);
     drawSprite(Image,destSurface,x,y,0,0,w,h); 
     
     return destSurface;           
}

short int collision_detect(int x1,int y1,int x2,int y2,SDL_Surface* img1, SDL_Surface* img2)
{
  int left1, left2, over_left;
  int right1, right2, over_right;
  int top1, top2, over_top;
  int bottom1, bottom2, over_bottom;
  int over_width, over_height;
  int i, j;
  int pixel1x, pixel2x;
  int pixel1y, pixel2y;

  left1 = x1;
  left2 = x2;
  right1 = x1 + img1->w;
  right2 = x2 + img2->w;
  top1 = y1;
  top2 = y2;
  bottom1 = y1 + img1->h;
  bottom2 = y2 + img2->h;

  // Trivial rejections:
  if (bottom1 < top2) return(0);
  if (top1 > bottom2) return(0);
  
  if (right1 < left2) return(0);
  if (left1 > right2) return(0);


  // Ok, compute the rectangle of overlap:
  if (bottom1 > bottom2) over_bottom = bottom2;
  else over_bottom = bottom1;
 
  if (top1 < top2) over_top = top2;
  else over_top = top1;

  if (right1 > right2) over_right = right2;
  else over_right = right1;

  if (left1 < left2) over_left = left2;
  else over_left = left1;

  over_width = (over_right - over_left);
  over_height = (over_bottom - over_top);


  // Now compute starting offsets into both objects' bitmaps:

  pixel1y = (over_top - y1);
  pixel2y = (over_top - y2);

  pixel1x = (over_left - x1);
  pixel2x = (over_left - x2);

  
  // Now start scanning the whole rectangle of overlap,
  // checking the corresponding pixel of each object's
  // bitmap to see if they're both non-zero:

  SDL_LockSurface(img1);
  SDL_LockSurface(img2);

  for (i=0; i < over_height; i++) {
    for (j=0; j < over_width; j++) {
      if ( ((getpixel(img1, pixel1x + j, pixel1y + i)) > 0 ) &&
	   ((getpixel(img2, pixel2x + j, pixel2y + i)) > 0 ) ) {

	SDL_UnlockSurface(img1);
	SDL_UnlockSurface(img2);
	return(1);
      }
    }
  }


  // Worst case!  We scanned through the whole darn rectangle of overlap 
  // and couldn't find a single colliding pixel!

  SDL_UnlockSurface(img1);
  SDL_UnlockSurface(img2);

  return(0);

}

void generateAliens()
{
     int type=0;
     int levelcount=level+1;
     int typecount=0;
     int alien=0;
     veloc_type mod;
     int x=0,y=0;
     int nextpos=0;
     
     while(levelcount>0)
     {
         aliens[alien].alive=1;
         aliens[alien].object=type;
         aliens[alien].IsExplosion=0;
         aliens[alien].animcount=0;
         if(nextpos==0)
         {
             x=0; y=20;
         } else if(nextpos==1)
         {
             x=(screen->w-alienimg[type]->w)/2;
             y=20;
         } else if(nextpos==2)
         {
             x=(screen->w-alienimg[type]->w);
             y=20;
         } else if(nextpos==3)
         {
             x=0;
             y=(screen->h-alienimg[type]->h);
         } else if(nextpos==4)
         {
             x=(screen->w-alienimg[type]->w)/2;
             y=(screen->h-alienimg[type]->h);
         } else if(nextpos==5)
         {
             x=(screen->w-alienimg[type]->w);             
             y=(screen->h-alienimg[type]->h);
         } else if(nextpos==6)
         {
             x=0;
             y=(screen->h-alienimg[type]->h)/2;
         } else if(nextpos==7)
         {
             x=(screen->w-alienimg[type]->w); 
             y=(screen->h-alienimg[type]->h)/2;
         }

         nextpos++;

         typecount++;
         if(typecount==3)
         {
             type++;
             if(type>2) type=0;
             typecount=0;
         }

         aliens[alien].x=x;
         aliens[alien].y=y;
         aliens[alien].angle=vGetRandom(15);
         mod=CalcVelocity(aliens[alien].angle);
         aliens[alien].xdir=mod.xmod;
         aliens[alien].ydir=mod.ymod;
         aliens[alien].object=type;
         aliens[alien].nexttime=vGetTickCount()+2000;
         
         levelcount--;
         alien++;
     }
}

void ResetGame()
{
     int i;
     
     player.x=screen->w/2;
     player.y=screen->h/2;
     player.xdir=0;
     player.ydir=0;
     player.angle=0;
     player.alive=1;
     player.IsExplosion=0;
     player.animcount=0;
               
     for(i=0;i<MAXPLAYERBULLETS+MAXALIENBULLETS;i++)
         aliens[i].alive=0;
         
     generateAliens();
}

veloc_type CalcVelocity(int angle)
{
    int xmod=0,ymod=0;
    veloc_type mod;

    switch(angle)
    {
          case 0:
               xmod=0;ymod=-2;
               break;
          case 1:
               xmod=1;ymod=-2;
               break;
          case 2:
               xmod=1;ymod=-1;
               break;
          case 3:
               xmod=2;ymod=-1;
               break;
          case 4:
               xmod=2;ymod=0;
               break;
          case 5:
               xmod=2;ymod=1;
               break;
          case 6:
               xmod=1;ymod=1;
               break;
          case 7:
               xmod=1;ymod=2;
               break;
          case 8:
               xmod=0;ymod=2;
               break;
          case 9:
               xmod=-1;ymod=2;
               break;
          case 10:
               xmod=-1;ymod=1;
               break;
          case 11:
               xmod=-2;ymod=1;
               break;
          case 12:
               xmod=-2;ymod=0;
               break;
          case 13:
               xmod=-2;ymod=-1;
               break;
          case 14:
               xmod=-1;ymod=-1;
               break;
          case 15:
               xmod=-1;ymod=-2;
               break;
    }
 
    mod.xmod=xmod;
    mod.ymod=ymod;
    
    return mod;   
}
void LoadFont(u8 size)
{
     font = Font_Load("data/gfx/simplefont.tga");
     font_inv = Font_Load("data/gfx/simplefont_inv.tga");
	//font = TTF_OpenFont("arial.ttf", size);
}

void drawSprite(SDL_Surface* imageSurface, SDL_Surface* screenSurface, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
	SDL_Rect srcRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;

	SDL_Rect dstRect;
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width;
	dstRect.h = height;

	if(!(SDL_BlitSurface(imageSurface, &srcRect, screenSurface, &dstRect)))
	{
         if(strlen(SDL_GetError())>1)
               fprintf(stderr,"Error: %s\n",SDL_GetError());
    }
}

/* Add a bullet: */

void add_bullet(int x, int y, int a, int xm, int ym, int owner)
{
  int i, found;
  
  found = -1;
  
  for (i = 0; i < MAXPLAYERBULLETS+MAXALIENBULLETS && found == -1; i++)
    {
      if (bullets[i].timer <= 0)
	found = i;
    }
  
  if (found != -1)
    {
#ifndef EMBEDDED
      bullets[found].timer = 50;
#else
      bullets[found].timer = 30;
#endif
      
      bullets[found].x = x;
      bullets[found].y = y;
      bullets[found].owner = owner;      
      bullets[found].xdir = xm; //((fast_cos(a >> 3) * 5) >> 10) + (xm >> 4);
      bullets[found].ydir = ym; //- ((fast_sin(a >> 3) * 5) >> 10) + (ym >> 4);
      
      //playsound(SND_BULLET);
    }
}

#endif
