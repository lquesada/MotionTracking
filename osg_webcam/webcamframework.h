// Copyright 2010 Luis Quesada Torres
// elezeta@gmail.com

#ifndef __FRAMEWORK_H_
#define __FRAMEWORK_H_

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
#include <osg/ref_ptr>
#include <osgViewer/Viewer>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Node>
#include <osg/CameraNode>
#include <osgDB/ReadFile>
#include <iostream>
#include "thread.h"
#include "mutex.h"
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using namespace osg;
using namespace osgDB;
using namespace osgViewer;

#define PI 3.141592654f

#define FICHERO_IMAGEN 1
#define CAMARA_VIDEO 2

extern double COMPONENTE_NUEVA;
extern double COMPONENTE_ANTERIOR;
extern double DUREZA_ACTUA;
extern double UMBRAL;
extern int RESOLUTION_W;
extern int RESOLUTION_H;
extern int MOVEMENT_EXPLORE;
extern int MSPERFRAME;
extern int RESOLUTION_W_SHOW;
extern int RESOLUTION_H_SHOW;

extern osg::ref_ptr<osg::Texture2D> texture;
extern osg::ref_ptr<osgViewer::Viewer> viewer;
extern double actua_movx;
extern double actua_movy;

void init_viewer(osgViewer::Viewer &viewer);
osg::ref_ptr<osg::Node> CreateGeometry ();
void timer();
Matrix CTransform();


osg::ref_ptr<osg::Image> Convert_OpenCV_TO_OSG_IMAGE(IplImage* cvImg);

int initializeCapture(int modo,char *ruta);
int initializeCapture(int modo);
void captura();
void capturaTextura();
void inicializaCaptura();
void procesaCambios();
void actualizawebcam(void *thread);
void procesawebcam(void *thread);
int main( int, char** );

#endif
