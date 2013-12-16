#include "webcamframework.h"

osg::ref_ptr<osg::Texture2D> texture;

CvCapture* capture = 0;
IplImage* frame = 0;
osg::ref_ptr<osg::Image> frames = 0;
osg::ref_ptr<osg::Image> framesafe = 0;
char *rutas;
int modos = 0;
char hechaimagen = 0;
osg::ref_ptr<osg::Image> nueva = 0;
osg::ref_ptr<osg::Image> ultima = 0;
double recuerda_movx = 0;
double recuerda_movy = 0;
double actua_movx = 0;
double actua_movy = 0;
Mutex mpcambios;
int ready = 0;

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

osg::ref_ptr<osg::Image> Convert_OpenCV_TO_OSG_IMAGE(IplImage* cvImg)
{

	if(cvImg->nChannels == 3)
	{
		osg::ref_ptr<osg::Image> osgImg = new osg::Image();

		osgImg->setImage(
			cvImg->width, //s
			cvImg->height, //t
			3, //r
			3, //GLint internalTextureformat, (GL_LINE_STRIP, 0x0003)
			GL_BGR, // GLenum pixelFormat, (GL_RGB, 0x1907)
			5121, // GLenum type, (GL_UNSIGNED_BYTE, 0x1401)
			(unsigned char*)(cvImg->imageData), // unsigned char* data
			osg::Image::NO_DELETE // AllocationMode mode (shallow copy)
			);//int packing=1); (???)

		return osgImg;
	}
	else {
		printf("Unrecognized image type");
		return 0;
	}

}

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

void capturaTextura() {
  if (modos == FICHERO_IMAGEN && hechaimagen == 1)
    return;
  captura();
  hechaimagen = 1;
  IplImage *newimage = cvCreateImage(cvSize(RESOLUTION_W,RESOLUTION_H), IPL_DEPTH_8U, 3);
  cvResize(frame,newimage,CV_INTER_CUBIC);
  frames = Convert_OpenCV_TO_OSG_IMAGE(newimage);
  osg::ref_ptr<osg::Image> aux = frames;
  mpcambios.lock();
  ultima = nueva;
  nueva = aux;
  mpcambios.unlock();
}

void inicializaCaptura() {
  if (!initializeCapture(CAMARA_VIDEO,0))
    initializeCapture(FICHERO_IMAGEN,(char *)"texturas/ajuste.png");
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void procesaCambios() {
  if (modos == FICHERO_IMAGEN)
    return;
  int x,y,i,j,foundx,foundy,sumai,sumaj;
  double dif,difr,difg,difb,diff,sumax,sumay,count,aux;
  count = 1;
  osg::ref_ptr<osg::Image> auxn;
  osg::ref_ptr<osg::Image> auxa;
  mpcambios.lock();
  auxn = nueva.get();
  auxa = ultima.get();
  mpcambios.unlock();

  if (ultima) {
    //Comparo new y last
    sumax = 0;
    sumay = 0;
    for (x = 0;x < RESOLUTION_W;x++) {
      for (y = 0;y < RESOLUTION_H;y++) {
        diff = 999999999;
        osg::Vec4 colnueva = auxn->getColor(osg::Vec2(x,y));
        foundx = 0;
        foundy = 0;
        for (i = -MOVEMENT_EXPLORE;i <= MOVEMENT_EXPLORE;i++) {
          for (j = -MOVEMENT_EXPLORE;j <= MOVEMENT_EXPLORE;j++) {
            if (x+i >= 0 && x+i< RESOLUTION_W && y+j >= 0 && y+j< RESOLUTION_H) {
              osg::Vec4 colultima = auxa->getColor(osg::Vec2(x+i,y+j));
              difr = ((colnueva.r()-colultima.r())*255);
              difg = ((colnueva.g()-colultima.g())*255);
              difb = ((colnueva.b()-colultima.b())*255);
              if (difr < 0) difr *= -1;
              if (difg < 0) difg *= -1;
              if (difb < 0) difb *= -1;
              sumai = i*2;
              sumaj = j*2;
              if (sumai < 0)
                sumai *= -1;
              if (sumaj < 0)
                sumaj *= -1;
              dif = difr+difg+difb+sumai+sumaj;
              if (i == 0)
                dif *=0.5;
              if (j == 0)
                dif *=0.5;
              if (dif <= diff) {
                diff = dif;
                foundx = i;
                foundy = j;
              }
              //printf("%d %d %f   --> %d %d\n",i,j,dif,foundx,foundy);
            }
          }
        }
        if (foundx != 0 && foundy != 0) {
          sumax += foundx;
          sumay += foundy;
          count++;
        }
      }
    }
    aux = (sumax>0?sumax:-sumax)+(sumay>0?sumay:-sumay);
    if (aux < UMBRAL) {
      sumax = 0;
      sumay = 0;
    }
    sumax /= (double)count;
    sumay /= (double)count;
    recuerda_movx = COMPONENTE_NUEVA*((double)sumax)+COMPONENTE_ANTERIOR*recuerda_movx;
    recuerda_movy = COMPONENTE_NUEVA*((double)sumay)+COMPONENTE_ANTERIOR*recuerda_movy;
    actua_movx = DUREZA_ACTUA*recuerda_movx+(1-DUREZA_ACTUA)*actua_movx;
    actua_movy = DUREZA_ACTUA*recuerda_movy+(1-DUREZA_ACTUA)*actua_movy;
    //printf("x = %5f, y = %5f",actua_movx,actua_movy);
  }
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void actualizawebcam(void *thread) {
  inicializaCaptura();
  capturaTextura();
  Thread *thr = (Thread *)thread;
  while ((*thr).stopCheck()) {
    capturaTextura();
    framesafe=frames.get();
    if (ready<5)
      ready++;
  }
}

void procesawebcam(void *thread) {
  Thread *thr = (Thread *)thread;
  while (ready < 5)
    usleep(1000);
  while ((*thr).stopCheck()) {
    procesaCambios();
  }
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

char acumtime() {
  static int tvultimo = -1;
  static int timeacum = 0;
  int tvactual;
  int diferencia;
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  tvactual = (tv.tv_sec%5)*1000000+tv.tv_usec;
  diferencia = ((tvactual-tvultimo)+5000000)%5000000;
  timeacum += diferencia;
  tvultimo = tvactual;
  if (timeacum > MSPERFRAME) {
    timeacum %= MSPERFRAME;
    return 1;
  }
  else {
    return 0;
  }
}

class timerx : public osg::NodeCallback {
  public:
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};

void timerx::operator()(osg::Node* node, osg::NodeVisitor* nv) {
  if (acumtime()) {
    timer();
    timer();
    timer();
    texture->setImage (framesafe);
  }
}

int main( int, char** )
{
  ref_ptr<Viewer> viewer = new Viewer;
  texture = new osg::Texture2D;
  frames = new osg::Image;
  texture->setImage(frames);
  Thread actualizadorawebcam;
  actualizadorawebcam.start(&actualizawebcam,0);
  Thread procesadorawebcam;
  procesadorawebcam.start(&procesawebcam,0);
  osg::ref_ptr<osg::Node> geode (CreateGeometry().get());
  geode.get()->setUpdateCallback(new timerx);
  viewer->setSceneData(geode.get());
  init_viewer(*viewer);
  viewer->run();
  procesadorawebcam.stop();
  actualizadorawebcam.stop();
  procesadorawebcam.join();
  actualizadorawebcam.join();
}

