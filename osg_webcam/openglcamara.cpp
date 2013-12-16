#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <vector>
#include <list>
#include <string>

using namespace std;

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

CvCapture* capture = 0;
IplImage* frame = 0;
char *rutas;
int modos = 0;
GLuint texture;
char hechaimagen = 0;
#define FICHERO_IMAGEN 1
#define CAMARA_VIDEO 2


int initializeCapture(int modo,char *ruta);
int initializeCapture(int modo){
  initializeCapture(modo,0);
}

int initializeCapture(int modo,char *ruta){
  rutas = ruta;
  modos = modo;
  if(!capture){
    if(modo == CAMARA_VIDEO)
      capture = cvCaptureFromCAM(0);
    else if (modo == FICHERO_IMAGEN)
      return 1;
  }
  if( !capture ) {
    return 0;
  }
  return 1;
}

void captura() {
  frame = 0;
  if (modos == FICHERO_IMAGEN)
    frame = cvLoadImage(rutas, 1 );
  else
    frame = cvQueryFrame( capture );
  cvFlip(frame, NULL, 0);
  cvFlip(frame, NULL, 1);
}

void leeimagen(char *ruta) {
  frame = 0;
  if (modos == FICHERO_IMAGEN)
    frame = cvLoadImage(ruta, 1 );
  cvFlip(frame, NULL, 0);
}

void capturaTextura(GLuint &textur) {
  if (modos == FICHERO_IMAGEN && hechaimagen == 1)
    return;
  GLuint older;
  GLint older2;
  glGetIntegerv(GL_TEXTURE_2D,&older2);
  older = (GLuint)older2;
  glBindTexture( GL_TEXTURE_2D, textur );
  captura();
  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, frame->width, frame->height,
                       GL_RGB, GL_UNSIGNED_BYTE, frame->imageData );
  glBindTexture( GL_TEXTURE_2D, older );
  hechaimagen = 1;
}

void leeImagenTextura(GLuint &textur,char *ruta) {
  GLuint older;
  GLint older2;
  glGetIntegerv(GL_TEXTURE_2D,&older2);
  older = (GLuint)older2;
  glBindTexture( GL_TEXTURE_2D, textur );
  leeimagen(ruta);
//  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, frame->width, frame->height,
//                       GL_RGB, GL_UNSIGNED_BYTE, frame->imageData );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, frame->width, frame->height,
                       0, GL_RGB, GL_UNSIGNED_BYTE, frame->imageData );

  glBindTexture( GL_TEXTURE_2D, older );
}

void inicializaCaptura() {
  if (!initializeCapture(CAMARA_VIDEO,0))
    initializeCapture(FICHERO_IMAGEN,(char *)"texturas/ajuste.png");
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

#define WIDTH 800
#define HEIGHT 800
#define TIMEUPDATE 30
#define ROTATION 3

int w = WIDTH;
int h = HEIGHT;

double rotx = 0;

static void Init(void) {
  glClearIndex(0.0);
  glShadeModel(GL_FLAT);
  glOrtho(0.0,1.0,0.0,1.0,0.0,1.);
}

static void Display(void) {
  glClearColor(0.0,0.0,1.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glFrustum (-0.3, 0.3, -0.3, 0.3,1, 100.0);
  gluLookAt ( 1,2,5,0,0,0,0,1.0,0);

  glPushMatrix();

  capturaTextura(texture);
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture);
  glRotatef(rotx,0,1,0);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.9f, -0.9f, 0.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.9f, -0.9f, 0.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.9f, 0.9f, 0.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.9f, 0.9f, 0.0f);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  rotx += ROTATION;
  glFlush();
  glutSwapBuffers();
}

static void Reshape(int width,int height) {
  w = width;
  h = height;
  glViewport(0,0,(GLint)width,(GLint)height);
  
  Display();
}

static void Keyboard(unsigned char key,int x,int y) {
  switch (key) {
    case 27:
      exit(0);
  }
}

static void Timer(int a) {
  Display();
  glutTimerFunc(TIMEUPDATE,Timer,0);
}

int main(int argc,char *argv[]) {

  inicializaCaptura();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowPosition(50,50);
  glutInitWindowSize(WIDTH,HEIGHT);
  glutCreateWindow("Ejemplo");
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glGenTextures( 1, &texture );
  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutTimerFunc(TIMEUPDATE,Timer,0);
  glutMainLoop();
  return 0;
}

