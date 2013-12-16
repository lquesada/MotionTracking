#include "image.h"
#include "matrix.h"
#include "inputcamera.h"
#include "outputscreen.h"
#include "functions.h"
#include "config.h"
#include "point.h"

void drawScreen(outputscreen *a,point *center,rectangle *area,matrix *b,matrix *c,matrix *d,matrix *e,matrix *f,int timeStarted,int timeCaptured,int timeProcessed,int timeScreen) {
  static int timeStartedOld;
  static int timeCapturedOld;
  static int timeProcessedOld;
  static int x = -1,y = -1,z = -1;
  int tx;
  int ty;
  int tz;
  if (center->valid) {
    tx = (center->x-320)*2;
    ty = (-center->y+240)*2;
    tz = (-area->ar+90)*20;
    if (x == -1) {
      x=tx;
      y=ty;
      z=tz;
    }
    else {
      x=tx*TARGET_FACTOR_X+x*(1-TARGET_FACTOR_X);
      y=ty*TARGET_FACTOR_Y+y*(1-TARGET_FACTOR_Y);
      z=tz*TARGET_FACTOR_Z+z*(1-TARGET_FACTOR_Z);
    }
  }
  else {
    x*=TARGET_LOST_X;
    y*=TARGET_LOST_Y;
    z*=TARGET_LOST_Z;
  }
  drawAxis(a,x,y,z,480,480);
//filtered
//edges
//filteredt-1
//diff
  drawMatrix(a,b,0,0);
  drawMatrix(a,c,640,0);
  drawMatrix(a,d,1280,0);
  drawMatrix(a,e,640,480);
  drawMatrix(a,f,1280,480);
//  drawPoint(a,center);
//  drawRectangle(a,area);
//  drawPolygon(a,area);
  drawSubSampledMatrix(a,c,0,480,4);
  drawSubSampledMatrix(a,d,160,480,4);
  drawSubSampledMatrix(a,e,320,480,4);
//  drawConsole(a,640,0,416,600);
  char text[200];
/*  snprintf(text,200,"Tcapture = %1.4fs",(double)(timeCaptured-timeStarted)/1000.);
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
  drawTextSolid(a,text,1,76);*/
  flip(a);
  timeStartedOld = timeStarted;
  timeCapturedOld = timeCaptured;
  timeProcessedOld = timeProcessed;
}

void message(outputscreen *a,char *string) {
  static char *last;
  if (string!=last) {
    showMessage(a,string);
    printf(string);
    printf("\n");
  }
  last = string;
}

int main(int argc,char *argv[]) {
  int keepon = 1;
  int timeStarted = 0;
  int timeCaptured = 0;
  int timeProcessed = 0;
  int timeScreen = 0;
  int event;

  inputcamera cam = {0};
  outputscreen sc = {0};
  matrix input = {0};
  matrix inputnorm = {0};
  matrix latest = {0};
  matrix latestnorm = {0};
  matrix sob = {0};
  matrix sobnorm = {0};
  matrix sobsq = {0};
  matrix diskfilter = {0};
  matrix diskfilter2 = {0};
  matrix dif = {0};
  matrix difa = {0};
  matrix difsobel = {0};
  point center = {0};
  point ncenter = {0};
  rectangle area = {0};
  double avgdif;
  double avgdifc;
  double avgdifcf;
  double avgdifsobel;
  int diffCenter;
  int ite;
  int inside;
  int lastcolor;
  int actualcolor;
  int difcolor;

  int relocate = 0;
  int lostframes = 0;
  int lostmframes = 0;
  int wait = WAIT_TICKS;

  message(&sc,"");
  message(&sc,"###############################");
  message(&sc,"#                             #");
  message(&sc,"# Depth Perception Experiment #");
  message(&sc,"#                             #");
  message(&sc,"###############################");
  message(&sc,"");
  message(&sc,"Luis Quesada Torres 2010");
  message(&sc,"www.elezeta.com");
  message(&sc,"elezeta@gmail.com");
  message(&sc,"");
  message(&sc,"Pressing F forces a center detection.");
  message(&sc,"Pressing C forces a center clear.");
  message(&sc,"");
  message(&sc,"Initializing screen.");
  startScreen(&sc,"Depth Perception Experiment",1920,960);
  message(&sc,"Screen initialized.");
  message(&sc,"");
  message(&sc,"Initializing video input.");
  startCamera(&cam); //TODO check
  message(&sc,"Video input initialized.");
  message(&sc,"");
  message(&sc,"Waiting for webcam shutter speed to adjust...");
  while (keepon) {
    timeStarted = getTime();
    captureCamera(&cam,&input);
    timeCaptured = getTime();
    ///////////////////
    diskFilter(&input,&diskfilter);
    diskFilter(&diskfilter,&diskfilter2);
    normalizeBrightness(&diskfilter2,&latest,&inputnorm);
    sobel(&diskfilter2,&sob);
    normalizePositive(&sob,&sobnorm);
    diff(&inputnorm,&latestnorm,&dif);
    absolute(&dif,&difa);
    averageThreshold(&difa,DIFF_THRESHOLD,&avgdif);
    if (area.valid) {
      averageThresholdCenter(&difa,DIFF_THRESHOLD,&area,AREA_MARGIN,&avgdifc);
      if (avgdif>0)
        avgdifcf = avgdifc/avgdif;
      if (avgdifcf < MIN_MOVEMENT_AROUND_CENTER) {
        if (lostmframes < MIN_MOVEMENT_FRAMES) {
          lostmframes++;
          message(&sc,"Not enough movement ratio.");
        }
        else {
          center.valid = 0;
          area.valid = 0;
          message(&sc,"Not enough movement ratio. Center lost.");
        }
      }
    }
    else
      avgdifc = avgdif;
    if (event == EVENT_CLEAR) {
      wait = 0;
      center.x = 0;
      center.y = 0;
      center.valid = 0;
      area.valid = 0;
      message(&sc,"Forcing drop of center position.");
    }
    if (event == EVENT_FORCE) {
      wait = 0;
      center.x = 320;
      center.y = 240;
      center.valid = 1;
      lostframes = 0;
      lostmframes = 0;
      message(&sc,"Forcing new center position.");
      neighborhoodColorAverage(&diskfilter2,&center,&lastcolor);
    }
    if (!wait) {
      if (!center.valid) {
        if (avgdif > CENTER_MINIMUM_AVGDIFF) {
          averagePositionWeight(&difa,&center);
          neighborhoodColorAverage(&diskfilter2,&center,&lastcolor);
          message(&sc,"New center and area found.");
          lostframes = 0;
          lostmframes = 0;
        }
        else 
          message(&sc,"No new center and area found.");
      }
      if (avgdif > MOVEMENT_MINIMUM_AVGDIFF) {
        if (center.valid) {
          lostframes = 0;
          ite = 0;
          neighborhoodColorAverage(&diskfilter2,&center,&actualcolor);
          difcolor=abs(actualcolor-lastcolor);
          if (difcolor>MAX_COLOR_DIFFERENCE) {
            center.valid = 0;
            area.valid = 0;
            message(&sc,"Too much color difference. Trying to relocate center.");
            relocate = 1;
          }
          do {
            message(&sc,"Recalibrating center.");
            radioCollision(&sobnorm,&center,RADIO_THRESHOLD,&area,&ncenter);
            pointDifference(&center,&ncenter,&diffCenter);
            if (area.valid) {
              pointInRectangle(&ncenter,&area,32,&inside);
              if (!inside) {
                center.valid = 0;
                area.valid = 0;
                message(&sc,"Out of previous area. Trying to relocate center.");
                relocate = 1;
              }
            }
            center = ncenter;
            ite++;
          } while (diffCenter>=MIN_CENTER_MOVEMENT && ite < MAX_CENTER_CALIBRATIONS);
          if (relocate) {
            neighborhoodSearch(&diskfilter2,&center,MAX_COLOR_STEPS,COLOR_SEPARATION,&lastcolor,&actualcolor);
            difcolor=abs(actualcolor-lastcolor);
            relocate = 0;
            if (difcolor>MAX_COLOR_DIFFERENCE) {
              center.valid = 0;
              area.valid = 0;
              message(&sc,"Center couldn't be relocated.");
            }
            else {
              center.valid = 1;
              radioCollision(&sobnorm,&center,RADIO_THRESHOLD,&area,&ncenter);
              center = ncenter;
              message(&sc,"Center relocated.");
              neighborhoodColorAverage(&diskfilter2,&center,&lastcolor);
              actualcolor=lastcolor;
            }
          }
          lastcolor=actualcolor; 
          if (area.ar < MINIMUM_AREA) {
            center.valid = 0;
            area.valid = 0;
            message(&sc,"Too small area. Center lost.");
          }
          if (area.ar > MAXIMUM_AREA) {
            center.valid = 0;
            area.valid = 0;
            message(&sc,"Too big area. Center lost.");
          }
        }
      }
      else if (center.valid) {
        lostframes++;
        if (lostframes >= CENTER_LOST_FRAMES) {
          center.valid = 0;
          area.valid = 0;
          message(&sc,"Not enough movement. Center lost.");
        }
        else {
          message(&sc,"Not enough movement.");
        }
      }
    }
    ///////////////////
    timeProcessed = getTime();
    drawScreen(&sc,
               &center,&area,
               &input,&inputnorm,&sobnorm,&latest,&difa,
               timeStarted,timeCaptured,timeProcessed,timeScreen);
    event = checkEvents();
    if (event==EVENT_EXIT)
      keepon=0;
    timeScreen = getTime();
    switchMatrix(&input,&latest);
    switchMatrix(&inputnorm,&latestnorm);
    if (wait > 0) {
      wait -= (timeScreen-timeStarted);
      if (wait < 0) {
        wait = 0; 
        message(&sc,"Algorithm running.");
        message(&sc,"");
      }
    }
  }
  message(&sc,"");
  message(&sc,"Freeing matrix resources.");
  freeMatrix(&input);
  freeMatrix(&inputnorm);
  freeMatrix(&latest);
  freeMatrix(&latestnorm);
  freeMatrix(&sob);
  freeMatrix(&sobnorm);
  freeMatrix(&sobsq);
  freeMatrix(&diskfilter);
  freeMatrix(&diskfilter2);
  freeMatrix(&dif);
  freeMatrix(&difa);
  freeMatrix(&difsobel);
  message(&sc,"Matrix resources freed (wake up, Neo).");
  message(&sc,"");
  message(&sc,"");
  message(&sc,"Freeing video input resources.");
  stopCamera(&cam);
  message(&sc,"Video input resources freed.");
  message(&sc,"");
  message(&sc,"Freeing screen resources.");
  stopScreen(&sc);
  message(&sc,"Screen resources freed.");
  message(&sc,"");
  message(&sc,"Exiting...");

}

