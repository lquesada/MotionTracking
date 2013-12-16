#include "outputscreen.h"

void startScreen(outputscreen *a,char *title,int w,int h) {
  SDL_Init(SDL_INIT_VIDEO);
  atexit(SDL_Quit);
  SDL_WM_SetCaption(title,title);
  a->screen = SDL_SetVideoMode(w,h, 24, SDL_DOUBLEBUF);
  TTF_Init();
  atexit(TTF_Quit);
  a->font = TTF_OpenFont("dejavusansmono.ttf",11);
}

void stopScreen(outputscreen *a) {
  if (a->font) {
    TTF_CloseFont(a->font);
    a->font = 0;
  }
  if (a->screen) {
    SDL_FreeSurface(a->screen);
    a->screen = 0;
  }
}

int checkEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event) > 0) {
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE)))
      return EVENT_EXIT;
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f)
      return EVENT_FORCE;
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_c)
      return EVENT_CLEAR;
  } 
  return EVENT_NOTHING;
}

int getTime() {
  return SDL_GetTicks();
}

void drawMatrix(outputscreen *a,matrix *m,int x,int y) {
  if (!m->d) return;
  SDL_Surface *aux = SDL_CreateRGBSurface(SDL_SWSURFACE,m->w,m->h,24,0,0,0,0);
  SDL_Rect dest_rect;
  int i,j;
  int pointer;
  int val;

  if (m->r) {
    for (i = 0;i < m->h;i++) {
      for (j = 0;j < m->w;j++) {
        pointer = j+i*m->w;
        *((unsigned char *)(aux->pixels)+pointer*3) = (unsigned char)m->r[pointer];
        *((unsigned char *)(aux->pixels)+pointer*3+1) = (unsigned char)m->g[pointer];
        *((unsigned char *)(aux->pixels)+pointer*3+2) = (unsigned char)m->b[pointer];
      }
    }
  }
  else {
    for (i = 0;i < m->h;i++) {
      for (j = 0;j < m->w;j++) {
        pointer = j+i*m->w;
        val = m->d[pointer];
        *((unsigned char *)(aux->pixels)+pointer*3) = (unsigned char)val;
        *((unsigned char *)(aux->pixels)+pointer*3+1) = (unsigned char)val;
        *((unsigned char *)(aux->pixels)+pointer*3+2) = (unsigned char)val;
      }
    }
  }
  dest_rect.x = x;
  dest_rect.y = y;
  SDL_BlitSurface(aux,NULL,a->screen,&dest_rect);
  SDL_FreeSurface(aux);
}

void drawPoint(outputscreen *a,point *p) {
  if (p->valid)
    filledCircleRGBA(a->screen,p->x,p->y,4,255,0,0,255);
}

void drawRectangle(outputscreen *a,rectangle *r) {
  if (r->valid)
    rectangleRGBA(a->screen,r->x,r->y,r->x+r->w,r->y+r->h,255,0,0,255);
}

void drawPolygon(outputscreen *a,rectangle *r) {
  int i;
  char buf[20];
  if (r->valid) {
    for (i = 0;i < 32;i++) {
//      sprintf(buf,"%d\n",i);
//      drawTextSolid(a,buf,r->px[i%32],r->py[i%32]);
      lineRGBA(a->screen,r->px[i],r->py[i],r->px[(i+1)%32],r->py[(i+1)%32],255,0,0,255); 
    }
  }
}

void drawSubSampledMatrix(outputscreen *a,matrix *m,int x,int y,int factor) {
  if (!m->d) return;
  SDL_Surface *aux = SDL_CreateRGBSurface(SDL_SWSURFACE,m->w/factor,m->h/factor,24,0,0,0,0);
  SDL_Rect dest_rect;
  int i,j;
  int pointer;
  int val;
  int idst = 0;
  int jdst = 0;
  int nw = m->w/factor;
  if (m->r) {
    for (i = factor/2;i < m->h;i+=factor) {
      jdst = 0;
      for (j = factor/2;j < m->w;j+=factor) {
        pointer = jdst+idst*nw;
        *((unsigned char *)(aux->pixels)+pointer*3) = (unsigned char)m->r[j+i*m->w];
        *((unsigned char *)(aux->pixels)+pointer*3+1) = (unsigned char)m->g[j+i*m->w];
        *((unsigned char *)(aux->pixels)+pointer*3+2) = (unsigned char)m->b[j+i*m->w];
        jdst++;
      }
      idst++;
    }
  }
  else {
    for (i = factor/2;i < m->h;i+=factor) {
      jdst = 0;
      for (j = factor/2;j < m->w;j+=factor) {
        pointer = jdst+idst*nw;
        val = m->d[j+i*m->w];
        *((unsigned char *)(aux->pixels)+pointer*3) = (unsigned char)val;
        *((unsigned char *)(aux->pixels)+pointer*3+1) = (unsigned char)val;
        *((unsigned char *)(aux->pixels)+pointer*3+2) = (unsigned char)val;
        jdst++;
      }
      idst++;
    }
  }
  dest_rect.x = x;
  dest_rect.y = y;
  SDL_BlitSurface(aux,NULL,a->screen,&dest_rect);
  SDL_FreeSurface(aux);
}

void drawTextSolid(outputscreen *a,const char *string,int x,int y) {
  SDL_Color blue = {0,0,255};
  SDL_Surface *textSurface;
  textSurface = TTF_RenderText_Solid(a->font,string,blue);
  SDL_Rect textLocation = { x, y, 0, 0 };
  SDL_BlitSurface(textSurface, NULL, a->screen, &textLocation);
  SDL_FreeSurface(textSurface);
}

void drawTextWhite(outputscreen *a,const char *string,int x,int y) {
  SDL_Color white = {225,225,225};
  SDL_Surface *textSurface;
  textSurface = TTF_RenderText_Solid(a->font,string,white);
  SDL_Rect textLocation = { x, y, 0, 0 };
  SDL_BlitSurface(textSurface, NULL, a->screen, &textLocation);
  SDL_FreeSurface(textSurface);
}

void flip(outputscreen *a) {
  SDL_Flip(a->screen);
}

void showMessage(outputscreen *a,char *string) {
  int i;
  for (i = MAX_MSGS-1;i > 0;i--)
    a->messages[i] = a->messages[i-1];
  a->messages[i] = string;
}

void drawAxis(outputscreen *a,int ox,int oy,int oz,int x,int y) {
  SDL_Surface *screen = a->screen;
  boxRGBA(screen,x,y,x+160,y+120,10,10,10,255);
  double factorx = 0.2;
  double factory = 0.2;
  double factorz = 0.2;
  double factorr = 1;
  int avancex = ox*factorx;
  int avancey = oy*factory;
  int avancez = (oz*factorz)/2;
  double radio = (((double)(oz)+1000.)/1400.*6.3)+0.7;
  int color = ((avancez+50.)/150.*185.)+70.;
  if (color < 0)
    color = 0;
  if (color > 255)
    color = 255;

  //ROJOAZUL ARRIBA
  lineRGBA(screen,x+80-avancez+avancex,y+60+avancez,x+80-avancez+avancex,y+60+avancez-avancey,150,150,150,255);
  //VERDEAZUL DERECHA
  lineRGBA(screen,x+80-avancez,y+60+avancez-avancey,x+80-avancez+avancex,y+60+avancez-avancey,150,150,150,255);
  //VERDEROJO ABAJO
  lineRGBA(screen,x+80+avancex,y+60-avancey,x+80-avancez+avancex,y+60+avancez-avancey,150,150,150,255);
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

void drawConsole(outputscreen *a,int x,int y,int w,int h) {
  int i;
  boxRGBA(a->screen,x,y,x+w,y+h,10,10,10,255);
  rectangleRGBA(a->screen,x+1,y+1,x+w-2,y+h-2,255,255,255,255);
  int dy = y+h-15-8;
  int dx = x+11;
  for (i = 0;i < MAX_MSGS;i++) {
    if (a->messages[i] && dy > y+2) {
      drawTextWhite(a,a->messages[i],dx,dy);
    }
    dy -= 15;
  }
}

