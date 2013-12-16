#include "InputCamera.h"

InputCamera::InputCamera() {
  path = 0;
  mode = 0;
  capture = 0;
  frame = 0;
  startCamera();
}

InputCamera::~InputCamera() {
  stopCamera();
}

int InputCamera::initializeCapture(int mode){
  initializeCapture(mode,0);
}

int InputCamera::initializeCapture(int mode,char *path){
  this->path = path;
  this->mode = mode;
  if(!capture){
    if(mode == MODE_CAMERA)
      capture = cvCaptureFromCAM(0);
    else if (mode == MODE_FILE) {
      frame = cvLoadImage(path,1);
//      cvFlip(frame, NULL, 0);
      cvFlip(frame, NULL, 1);
    }
  }
  if( !capture ) {
    return 0;
  }
  return 1;
}

void InputCamera::startCamera() {
  if (!initializeCapture(MODE_CAMERA,0))
    initializeCapture(MODE_FILE,(char *)"default.png");
}

Image<RGBPixel> *InputCamera::captureCamera(Image<RGBPixel> **reuse) {
  char *idata;
  int pointer;
  if (mode == MODE_CAMERA) {
    frame = cvQueryFrame(capture);
    //cvFlip(frame, frame, 0);
    cvFlip(frame, frame, 1);
  }
  idata = frame->imageData;
  Image<RGBPixel> *img;
  if ((reuse) && (*reuse) && (frame->width==(**reuse).getW() && frame->height==(**reuse).getH())) {
      img = *reuse;
  }
  else {
    *reuse = new Image<RGBPixel>;
    (**reuse).allocate(frame->width,frame->height);
    img = *reuse;
  }
  //memcpy((*img).getData(),idata,(*img).getH()*(*img).getW()*3);
  for (int i = 0;i < (*img).getH();i++) {
    for (int j = 0;j < (*img).getW();j++) {
      pointer = (j+i*(*img).getW())*3;
      (*img).get(j,i).setRGB((unsigned char)idata[pointer+2],(unsigned char)idata[pointer+1],(unsigned char)idata[pointer]);
    }
  }
  return img;
}

void InputCamera::stopCamera() {
  if (frame)
    cvReleaseImage(&frame);
  if (capture)
    cvReleaseCapture(&capture);
}

