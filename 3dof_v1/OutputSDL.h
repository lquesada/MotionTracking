#ifndef __OUTPUTSDL_H__
#define __OUTPUTSDL_H__

#include "Image.template"
#include "RGBPixel.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "VMatrix.h"
#include "Output.h"
#include "Axis.h"

class OutputSDL {

  SDL_Surface *screen;

  SDL_Surface *newOrReuse(SDL_Surface **reuse,int w,int h);

  TTF_Font* font;

  public:

  OutputSDL(SDL_Surface *screen);
  ~OutputSDL();
  SDL_Surface *createSDLSurface(const Image<RGBPixel> &source,SDL_Surface **reuse); 
  void drawSurface(SDL_Surface *sur,int x,int y);
  SDL_Surface *drawImage(Image<RGBPixel> *im,int x,int y,SDL_Surface **reuse);
  void drawVectores(const VMatrix &vm,int x,int y,int wsepar,int hsepar,int woffset,int hoffset);
  void drawTextSolid(const char *string,int x,int y,SDL_Color &fg);
  void drawTextShaded(const char *string,int x,int y,SDL_Color &fg,SDL_Color &bg);
  void drawCenter(const Output &o,int srcw,int srch,int x,int y,int dstw,int dsth,int ratio);
  void drawAxis(const Axis &a,int x,int y,int w,int h);
};

#endif
