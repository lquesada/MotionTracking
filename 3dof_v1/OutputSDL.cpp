#include "OutputSDL.h"

OutputSDL::~OutputSDL() {
  if (font)
    TTF_CloseFont(font);
}

OutputSDL::OutputSDL(SDL_Surface *screen) {
  TTF_Init();
  atexit(TTF_Quit);
  this->screen = screen;
  font = TTF_OpenFont("dejavusansmono.ttf",11);
}

SDL_Surface *OutputSDL::newOrReuse(SDL_Surface **reuse,int w,int h) {
  if ((reuse) && (*reuse) && (w==(*reuse)->w && h==(*reuse)->h))
    return *reuse;
  else {
    if ((reuse) && (*reuse))
      SDL_FreeSurface(*reuse);
    *reuse = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0,0,0,0);
    return *reuse;
  }
}

SDL_Surface *OutputSDL::createSDLSurface(const Image<RGBPixel> &source,SDL_Surface **reuse) {
  SDL_Surface *ret = newOrReuse(reuse,source.getW(),source.getH());
  int pointer;
  for (int i = 0;i < source.getH();i++) { 
    for (int j = 0;j < source.getW();j++) {
      pointer = (j+i*source.getW())*3;
      *((unsigned char *)(ret->pixels)+pointer+2) = source.get(j,i).getR();
      *((unsigned char *)(ret->pixels)+pointer+1) = source.get(j,i).getG();
      *((unsigned char *)(ret->pixels)+pointer) = source.get(j,i).getB();
    }
  }
  //memcpy(ret->pixels,source.getData(),source.getH()*source.getW()*3);
  return ret;
}

void OutputSDL::drawSurface(SDL_Surface *sur,int x,int y) {
  SDL_Rect source_rect,dest_rect;
  source_rect.x = 0;
  source_rect.y = 0;
  source_rect.w = sur->w;
  source_rect.h = sur->h;
  dest_rect.x = x;
  dest_rect.y = y;
  SDL_BlitSurface(sur, &source_rect, screen, &dest_rect);
}
SDL_Surface *OutputSDL::drawImage(Image<RGBPixel> *im,int x,int y,SDL_Surface **reuse) {
  SDL_Rect source_rect,dest_rect;
  source_rect.x = 0;
  source_rect.y = 0;
  source_rect.w = (*im).getW();
  source_rect.h = (*im).getH();
  dest_rect.x = x;
  dest_rect.y = y;
  SDL_Surface *sur = createSDLSurface(*im,reuse);
  SDL_BlitSurface(sur, &source_rect, screen, &dest_rect);
  return sur;
}

void OutputSDL::drawVectores(const VMatrix &vm,int x,int y,int wsepar,int hsepar,int woffset,int hoffset) {
  int originx;
  int originy;
  int targetx;
  int targety;
  for (int i = 0;i < vm.getH();i++) {
    for (int j = 0;j < vm.getW();j++) {
      originy = y+hoffset+hsepar*i;
      originx = x+woffset+wsepar*j;
      targety = originy+(vm.get(j,i).getH())*((double)hsepar)/255.;
      targetx = originx+(vm.get(j,i).getV())*((double)wsepar)/255.;
      if (originx != targetx || originy != targety) {
        lineRGBA(screen,originx,originy,targetx,targety,255,0,0,255);
      }
      else {
        pixelRGBA(screen,originx,originy,255,255,255,50);
      }
    }
  }
}

void OutputSDL::drawTextSolid(const char *string,int x,int y,SDL_Color &fg) {
  SDL_Surface *textSurface;
  textSurface = TTF_RenderText_Solid(font,string, fg); 
  SDL_Rect textLocation = { x, y, 0, 0 };
  SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
  SDL_FreeSurface(textSurface);
}

void OutputSDL::drawTextShaded(const char *string,int x,int y,SDL_Color &fg,SDL_Color &bg) {
  SDL_Surface *textSurface;
  textSurface = TTF_RenderText_Shaded(font,string,fg,bg); 
  SDL_Rect textLocation = { x, y, 0, 0 };
  SDL_BlitSurface(textSurface, NULL,screen, &textLocation);
  SDL_FreeSurface(textSurface);
}

void OutputSDL::drawCenter(const Output &o,int srcw,int srch,int x,int y,int dstw,int dsth,int ratio) {
  if (o.valid)
    filledCircleRGBA(screen,x+o.positioncenterscreen.x*dstw/srcw,y+o.positioncenterscreen.y*dsth/srch,4*ratio,255,0,0,255);
}

void OutputSDL::drawAxis(const Axis &a,int x,int y,int w,int h) {
  boxRGBA(screen,x,y,x+w,y+h,10,10,10,255);
  double factorx = 0.2;
  double factory = 0.2;
  double factorz = 0.2;
  double factorr = 3;
  int avancex = a.x*factorx;
  int avancey = a.y*factory;
  int avancez = (a.z*factorz)/2;
  double radio = ((avancez+50)/150*6.3)+0.7;
  int color = ((avancez+50.)/150.*185.)+70.;

  //DESDE U HASTA U+X EN AZUL
  lineRGBA(screen,x+80-avancez,y+60+avancez,x+80+avancex-avancez,y+60+avancez,0,0,255,255);
  //DESDE J HASTA J+U EN ROJO
  lineRGBA(screen,x+80+avancex,y+60,x+80-avancez+avancex,y+60+avancez,255,0,0,255);
  //DESDE K HASTA K+U EN VERDE
  lineRGBA(screen,x+80-avancez,y+60+avancez-avancey,x+80,y+60-avancey,0,255,0,255);
  //ROJO ARRIBA
  lineRGBA(screen,x+80+avancex,y+60,x+80+avancex,y+60-avancey,255,0,0,255);
  //AZUL ARRIBA
  lineRGBA(screen,x+80-avancez,y+60+avancez,x+80-avancez,y+60+avancez-avancey,0,0,255,255);
  //VERDE DERECHA
  lineRGBA(screen,x+80,y+60-avancey,x+80+avancex,y+60-avancey,0,255,0,255);
  //ROJOAZUL ARRIBA
  lineRGBA(screen,x+80-avancez+avancex,y+60+avancez,x+80-avancez+avancex,y+60+avancez-avancey,150,150,150,255);
  //VERDEAZUL DERECHA
  lineRGBA(screen,x+80-avancez,y+60+avancez-avancey,x+80-avancez+avancex,y+60+avancez-avancey,150,150,150,255);
  //VERDEROJO ABAJO
  lineRGBA(screen,x+80+avancex,y+60-avancey,x+80-avancez+avancex,y+60+avancez-avancey,150,150,150,255);
   
  lineRGBA(screen,x+80+avancex,y+60-2,x+80+avancex,y+60+2,255,0,0,255);
  lineRGBA(screen,x+80-2,y+60-avancey,x+80+2,y+60-avancey,0,255,0,255);
  lineRGBA(screen,x+80-avancez+1,y+60+avancez+1,x+80-avancez-1,y+60+avancez-1,0,0,255,255);

  lineRGBA(screen,x+80-70,y+60,x+80,y+60,100,0,0,255);
  lineRGBA(screen,x+80,y+60,x+80+70,y+60,200,0,0,255);
  lineRGBA(screen,x+80,y+60,x+80,y+60+50,0,100,0,255);
  lineRGBA(screen,x+80,y+60-50,x+80,y+60,0,200,0,255);
  lineRGBA(screen,x+80,y+60,x+80+40,y+60-40,0,0,100,255);
  lineRGBA(screen,x+80,y+60,x+80-40,y+60+40,0,0,200,255);

  filledCircleRGBA(screen,x+80+avancex-avancez,y+60-avancey+avancez,radio*factorr,color,color,color,255);
}
