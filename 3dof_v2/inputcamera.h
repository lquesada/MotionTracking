#ifndef __INPUTCAMERA_H__
#define __INPUTCAMERA_H__

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "matrix.h"

#define MODE_CAMERA 0
#define MODE_FILE 1

typedef struct inputcamera {
  int mode;
  CvCapture* capture;
  IplImage* frame;
} inputcamera;

void startCamera(inputcamera *a);
void stopCamera(inputcamera *a);
void captureCamera(inputcamera *a,matrix *b);

#endif
