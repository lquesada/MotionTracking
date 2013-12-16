#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "point.h"

typedef struct rectangle {
  int x;
  int y;
  int w;
  int h;
  int a;
  int ar;
  int valid;
  int px[32];
  int py[32];
} rectangle;

void pointInRectangle(point *a,rectangle *b,int nvert,int *inside);

#endif
