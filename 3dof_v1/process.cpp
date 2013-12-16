#include "RGBPixel.h"
#include "BWPixel.h"
#include "InputCamera.h"
#include "OutputSDL.h"
#include "Image.template"
#include "Filter.h"
#include "SDL/SDL.h"
#include "VMatrix.h"
#include "Output.h"
#include "Axis.h"


#define SCREEN_W        800
#define SCREEN_H        480

//640x480
//            160*120*5
//160x120 * 5

int main(int argc,char *argv[]) {
  SDL_Surface *screen;
  SDL_Event event;
  SDL_Color white = {255,255,255};
  SDL_Color black = {0,0,0};
  SDL_Color red = {255,0,0};
  SDL_Color green = {0,255,0};
  SDL_Color blue = {0,0,255};

  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 24, SDL_DOUBLEBUF);

  SDL_WM_SetCaption("Depth perception experiment", "Depth perception experiment");

  InputCamera cinp;
  OutputSDL couts(screen);

  int seguir = 1;

  Filter ones5;
  ones5.makeOnes(5);

  Filter ones3;
  ones3.makeOnes(3);

  Image<RGBPixel> *captada = 0;
  SDL_Surface *dCaptada = 0;
  Image<RGBPixel> *reducida = 0;
  Image<RGBPixel> *ureducida = 0;
  Image<RGBPixel> *reducidawork = 0;
  Image<RGBPixel> *ureducidawork = 0;
  Image<BWPixel> *center = 0;
  Image<RGBPixel> *centerb = 0;
  Image<RGBPixel> *aux = 0;
  SDL_Surface *dReducida = 0;
  Image<RGBPixel> *sobel = 0;
  Image<BWPixel> *movementim = 0;
  Image<BWPixel> *movementf = 0;
  Image<RGBPixel> *movementimb = 0;
  Image<RGBPixel> *sobelrel = 0;
  Image<RGBPixel> *sobelrelmov = 0;
  Image<RGBPixel> *usobel = 0;
  Axis axis;

  SDL_Surface *dSobel = 0;
  VMatrix *movement = new VMatrix;
  VMatrix *movementc = new VMatrix;
  Output globaloutput,output;
  int tprincipio = 0;
  int tgetimagen = 0;
  int tprocesa = 0;
  int tsdlini = 0;
  int tsdl = 0;
  char text[200];
  double factor;
  double fps;
  int norun = 0;
  while (seguir) {
    tprincipio = SDL_GetTicks();
    cinp.captureCamera(&captada);
    tgetimagen = SDL_GetTicks()-tprincipio;


    (*captada).fastShrink(&reducida);
    (*reducida).fastSobel(&sobel);
    if (ureducida) {
      (*reducida).normalizeAgainst(*ureducida,&reducidawork);
      (*movement).compare(*ureducida,*reducidawork,8,8,3,300,&movement);
      if (movement->total > 220 || !norun) {
        globaloutput.procesa(*movement,*sobel,8,8,ones5,1000,0.7,1.0,&center,&movementc);
        norun = 1;
      }
      else {
        globaloutput.valid = 0;
        globaloutput.movementscreen.x = 0;
        globaloutput.movementscreen.y = 0;
        globaloutput.movementscreen.z = 0;
      }
      output.soften(globaloutput,0.55,0.1,350);
      axis.update(output);
      (*center).superSample(8,8,&centerb);
      (*movementc).getImage(&movementim);
      (*movementim).filter(ones3,&movementf);
      (*movementf).superSample(8,8,&movementimb);
      (*sobel).highPassAverageFilter(20,&sobelrel);
      (*sobelrel).andFilter(*movementimb,&sobelrelmov);
    }

    printf("##  %6.2f %6.2f %6.2f\t|||\t%6.2f %6.2f %6.2f # %d\n",output.positioncenterscreen.x,output.positioncenterscreen.y,output.areascreen,output.movementscreen.x,output.movementscreen.y,output.movementscreen.z,movement->total);


    tsdlini = SDL_GetTicks();
    if (ureducida) {
      couts.drawImage(captada,0,0,&dCaptada);
      couts.drawImage(sobel,640,0,&dSobel);
      couts.drawImage(reducida,640,120,&dReducida);
      couts.drawVectores(*movement,640,120,8,8,4,4);
      couts.drawImage(sobelrelmov,640,240,&dReducida);
      couts.drawCenter(output,(*centerb).getW(),(*centerb).getH(),644,244,(*centerb).getW(),(*centerb).getH(),1);
      couts.drawAxis(axis,640,360,160,120);
    }
    snprintf(text,200,"Tcapture = %1.4fs",(double)(tgetimagen)/1000.);
    couts.drawTextSolid(text,1,1,blue);
    snprintf(text,200,"Tprocess = %1.4fs",(double)(tprocesa)/1000.);
    couts.drawTextSolid(text,1,16,blue);
    snprintf(text,200,"TSDL     = %1.4fs",(double)(tsdl)/1000.);
    couts.drawTextSolid(text,1,31,blue);
    snprintf(text,200,"Ttotal   = %1.4fs",(double)(tprocesa+tgetimagen+tsdl)/1000.);
    couts.drawTextSolid(text,1,46,blue);
    snprintf(text,200,"%1.4f FPS",1000./((double)(tprocesa+tgetimagen+tsdl)));
    couts.drawTextSolid(text,1,61,blue);
    fps = 1000./((double)(tprocesa));
    snprintf(text,200,"%1.4f MAX FPS",fps);
    couts.drawTextSolid(text,1,76,blue);
    SDL_Flip(screen);
    while (SDL_PollEvent(&event) > 0) {
      if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE)))
        seguir = 0;
    }
    tsdl = SDL_GetTicks()-tsdlini;
    tprocesa = SDL_GetTicks()-tprincipio-tgetimagen-tsdl;
    aux = ureducidawork;
    ureducidawork = reducidawork;
    reducidawork = aux;
    aux = ureducida;
    ureducida = reducida;
    reducida = aux;
    aux = usobel;
    usobel = sobel;
    sobel = aux;
  }
  if (captada)
    delete captada;
  if (dCaptada)
    SDL_FreeSurface(dCaptada);
  if (reducida)
    delete reducida;
  if (ureducida)
    delete ureducida;
  if (movementc)
    delete movementc;
  if (movementim)
    delete movementim;
  if (movementimb)
    delete movementimb;
  if (movementf)
    delete movementf;
  if (sobelrel)
    delete sobelrel;
  if (sobelrelmov)
    delete sobelrelmov;
  if (center)
    delete center;
  if (centerb)
    delete centerb;
  if (reducidawork)
    delete reducidawork;
  if (ureducidawork)
    delete ureducidawork;
  if (dReducida)
    SDL_FreeSurface(dReducida);
  if (sobel)
    delete sobel;
  if (usobel)
    delete usobel;
  if (dSobel)
    SDL_FreeSurface(dSobel);
  exit(0);
}
