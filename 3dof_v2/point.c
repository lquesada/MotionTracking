#include "point.h"

void neighborhoodColorAverage(matrix *a,point *p,int *color) {
  int i,j;
  i = p->y;
  j = p->x;
  if (p->x > 0 && p->x < a->w-1 && p->y > 0 && p->y < a->h-1) {
    *color =(
a->d[(i-1)*a->w+j-1] +	a->d[(i-1)*a->w+j]   +	a->d[(i-1)*a->w+j+1] +
a->d[(i)*a->w+j-1]   +	a->d[(i)*a->w+j]     +	a->d[(i)*a->w+j+1]   +
a->d[(i+1)*a->w+j-1] +	a->d[(i+1)*a->w+j]   +	a->d[(i+1)*a->w+j+1]
            )/9;
  }
}

void neighborhoodSearch(matrix *a,point *p,int steps,int separation,int *color,int *newcolor) {
  int min = 9999;
  int look;
  int i,j;
  point newcenter;
  point goodcenter;
  int difcolor;

  for (i = -steps;i <= steps;i++) {
    for (j = -steps;j <= steps;j++) {
      newcenter.x = p->x+j*separation;
      newcenter.y = p->y+i*separation;
      neighborhoodColorAverage(a,&newcenter,&look);
      difcolor=abs(look-*color);
      if (difcolor < min) {
        min = look;
        goodcenter = newcenter;
      }
    }
  }
  *newcolor = min;
  goodcenter.valid = 1;
}
