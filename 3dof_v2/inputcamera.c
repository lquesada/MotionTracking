#include "inputcamera.h"
#include "matrix.h"

int initializeCapture(inputcamera *a,int mode);

int initializeCapture(inputcamera *a,int mode){
  a->mode = mode;
  if(!a->capture){
    if(a->mode == MODE_CAMERA)
      a->capture = cvCaptureFromCAM(0);
    else if (a->mode == MODE_FILE) {
      a->frame = cvLoadImage("default.png",1);
//      cvFlip(frame, NULL, 0);
      cvFlip(a->frame, NULL, 1);
    }
  }
  if( !a->capture ) {
    return 0;
  }
  return 1;
}

void startCamera(inputcamera *a) {
  if (!initializeCapture(a,MODE_CAMERA))
    initializeCapture(a,MODE_FILE);
}

void stopCamera(inputcamera *a) {
  if (a->frame) {
    cvReleaseImage(&a->frame);
    a->frame = 0;
  }
  if (a->capture) {
    cvReleaseCapture(&a->capture);
    a->capture = 0;
  }
}

void captureCamera(inputcamera *a,matrix *b) {
  char *idata;
  int pointer,pointer2;
  int i,j;
  if (a->mode == MODE_CAMERA) {
    a->frame = cvQueryFrame(a->capture);
    //cvFlip(a->frame, a->frame, 0);
    cvFlip(a->frame, a->frame, 1);
  }
  idata = a->frame->imageData;
  allocateCMatrix(b,a->frame->width,a->frame->height);
  if (b->r) {
    for (i = 0;i < a->frame->height;i++) {
      for (j = 0;j < a->frame->width;j++) {
        pointer = (j+i*a->frame->width)*3;
        pointer2 = j+i*a->frame->width;
        b->r[pointer2] = ((int)((unsigned char)idata[pointer+0]));
        b->g[pointer2] = ((int)((unsigned char)idata[pointer+1]));
        b->b[pointer2] = ((int)((unsigned char)idata[pointer+2]));
        b->d[pointer2] = (b->r[pointer2]+b->g[pointer2]+b->b[pointer2])/3;
      }
    }
  }
  else {
    for (i = 0;i < a->frame->height;i++) {
      for (j = 0;j < a->frame->width;j++) {
        pointer = (j+i*a->frame->width)*3;
        b->d[j+i*a->frame->width] = ((int)((unsigned char)idata[pointer]+(unsigned char)idata[pointer+1]+(unsigned char)idata[pointer+2]))/3.;
      }
    }
  }
}
