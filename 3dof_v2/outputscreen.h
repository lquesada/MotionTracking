#ifndef __OUTPUTSCREEN_H__
#define __OUTPUTSCREEN_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_timer.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "matrix.h"
#include "config.h"
#include "point.h"
#include "rectangle.h"

#define EVENT_NOTHING 0
#define EVENT_EXIT 1
#define EVENT_FORCE 2
#define EVENT_CLEAR 3
#define MAX_MSGS 40

typedef struct outputscreen {
  SDL_Surface *screen;
  TTF_Font* font;
  char *messages[MAX_MSGS];
} outputscreen;

void startScreen(outputscreen *a,char *title,int w,int h);
void stopScreen(outputscreen *a);
int checkEvents();
int getTime();
void drawMatrix(outputscreen *a,matrix *m,int x,int y);
void drawPoint(outputscreen *a,point *p);
void drawRectangle(outputscreen *a,rectangle *r);
void drawSubSampledMatrix(outputscreen *a,matrix *m,int x,int y,int factor);
void drawTextSolid(outputscreen *a,const char *string,int x,int y);
void drawTextWhite(outputscreen *a,const char *string,int x,int y);
void flip(outputscreen *a);
void showMessage(outputscreen *a,char *string);
void drawAxis(outputscreen *a,int ox,int oy,int oz,int x,int y);
void drawConsole(outputscreen *a,int x,int y,int w,int h);

#endif
