#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "VMatrix.h"
#include "Image.template"
#include "RGBPixel.h"
#include "Filter.h"

struct val2i {
  int x;
  int y;

  val2i() { x = y = 0; }
};

struct val2d {
  double x;
  double y;

  val2d() { x = y = 0; }
};

struct val3i {
  int x;
  int y;
  int z;

  val3i() { x = y = z = 0; }
};

struct val3d {
  double x;
  double y;
  double z;

  val3d() { x = y = z = 0; }
};

class Output {

  Image<BWPixel> *newOrReuse(Image<BWPixel> **reuse,int w,int h) const;
  VMatrix *newOrReuse(VMatrix **reuse,int w,int h) const;
  int totalmovement;

  public:
  int valid;

  val2d positioncenterscreen;
  double areascreen;
  double targetareascreen;
  val3d movementscreen;

  val3d movementspace;

  Output() { areascreen = 0;valid=0; positioncenterscreen.x = 0;positioncenterscreen.y = 0; }
  Image<BWPixel> *procesa(const VMatrix &a,const Image<RGBPixel> &sobela,int wfactor,int hfactor,const Filter &f,int maxdist,double distfactor,double minmovactarea,Image<BWPixel> **reuse1,VMatrix **reuse2);


  void soften(const Output &n,double factor,double areafactor,int minimumtotalarea);

};

#endif
