#include "rectangle.h"

void pointInRectangle(point *a,rectangle *b,int nvert,int *inside) {
  int i, j;
  *inside = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((b->py[i]>a->y) != (b->py[j]>a->y)) &&
         (a->x < (b->px[j]-b->px[i]) * (a->y-b->py[i]) / (b->py[j]-b->py[i]) + b->px[i]) )
       *inside = !(*inside);
  }
}

