#include "image.h"
#include "matrix.h"
#include "inputcamera.h"
#include "outputscreen.h"

void drawScreen(outputscreen *a,matrix *b,int timeStarted,int timeCaptured,int timeProcessed,int timeScreen);

int main(int argc,char *argv[]) {
  int keepon = 1;
  int timeStarted = 0;
  int timeCaptured = 0;
  int timeProcessed = 0;
  int timeScreen = 0;

  matrix input = {0};
  inputcamera cam = {0};
  outputscreen sc = {0};

  startScreen(&sc,"Depth perception experiment",640,480);
  startCamera(&cam);
  while (keepon) {
    timeStarted = getTime();
    captureCamera(&cam,&input);
    timeCaptured = getTime();

    timeProcessed = getTime();
    drawScreen(&sc,&input,
               timeStarted,timeCaptured,timeProcessed,timeScreen);
    if (checkEvents()==EVENT_EXIT)
      keepon=0;
    timeScreen = getTime();
  }
  stopCamera(&cam);
  stopScreen(&sc);

}


void drawScreen(outputscreen *a,matrix *b,int timeStarted,int timeCaptured,int timeProcessed,int timeScreen) {
  static int timeStartedOld;
  static int timeCapturedOld;
  static int timeProcessedOld;
  char text[200];
  drawMatrix(a,b,0,0);
  snprintf(text,200,"Tcapture = %1.4fs",(double)(timeCaptured-timeStarted)/1000.);
  drawTextSolid(a,text,1,1);
  snprintf(text,200,"Tprocess = %1.4fs",(double)(timeProcessed-timeCaptured)/1000.);
  drawTextSolid(a,text,1,16);
  snprintf(text,200,"TSDL     = %1.4fs",(double)(timeScreen-timeProcessedOld)/1000.);
  drawTextSolid(a,text,1,31);
  snprintf(text,200,"Ttotal   = %1.4fs",(double)(timeScreen-timeStartedOld)/1000.);
  drawTextSolid(a,text,1,46);
  snprintf(text,200,"%1.4f FPS",1000./((double)(timeScreen-timeStartedOld)));
  drawTextSolid(a,text,1,61);
  snprintf(text,200,"%1.4f MAXFPS",1000./((double)(timeProcessed-timeCaptured)));
  drawTextSolid(a,text,1,76);
  flip(a);
  timeStartedOld = timeStarted;
  timeCapturedOld = timeCaptured;
  timeProcessedOld = timeProcessed;
}

