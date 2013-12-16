#include "RGBPixel.h"
#include "BWPixel.h"
#include "InputCamera.h"
#include "OutputSDL.h"
#include "Image.template"
#include "SDL/SDL.h"


#define SCREEN_W        640
#define SCREEN_H        480 

int main(int argc,char *argv[]) {
  SDL_Surface *screen;
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);

  screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 24, SDL_DOUBLEBUF);
  SDL_WM_SetCaption("Depth perception experiment", "Depth perception experiment");

  InputCamera cinp;
  OutputSDL couts(screen);

  int seguir = 1;
  Image<RGBPixel> *a = 0;
  SDL_Surface *b = 0;
  while (seguir) {

    a = cinp.captureCamera(&a);
    b = couts.drawImage(a,0,0,&b);

    SDL_Flip(screen);
    SDL_Delay(1);
    while (SDL_PollEvent(&event) > 0) {
      if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE)))
        seguir = 0;
    }

  }
  if (a)
    delete a;
  if (b)
    SDL_FreeSurface(b);
  exit(0);
}
