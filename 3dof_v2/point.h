#ifndef __POINT_H__
#define __POINT_H__

#include "matrix.h"

typedef struct point {
  int x;
  int y;
  int valid;
} point;

void neighborhoodColorAverage(matrix *a,point *p,int *color);
void neighborhoodSearch(matrix *a,point *p,int steps,int separation,int *color,int *newcolor);

#endif
