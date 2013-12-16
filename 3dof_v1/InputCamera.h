#ifndef __INPUTCAMERA_H__
#define __INPUTCAMERA_H__

#include "Image.template"
#include "RGBPixel.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

#define MODE_CAMERA 0
#define MODE_FILE 1

class InputCamera {

  char *path;
  int mode;
  CvCapture* capture;
  IplImage* frame;

  int initializeCapture(int mode);
  int initializeCapture(int mode,char *path);
  void startCamera();
  void stopCamera();

  public:

  InputCamera();
  ~InputCamera();
  Image<RGBPixel> *captureCamera(Image<RGBPixel> **reuse);

};

#endif
