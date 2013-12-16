#include "matrix.h"
#include <stdlib.h>

void allocateMatrix(matrix *a,int w,int h) {
  if (a->w != w || a->h != h) {
    freeMatrix(a);
    a->w = w;
    a->h = h;
    a->d = malloc(sizeof(int)*w*h);
    a->r = 0;
    a->g = 0;
    a->b = 0;
  }
}

void allocateCMatrix(matrix *a,int w,int h) {
  if (a->w != w || a->h != h) {
    if (a->d)
      free(a->d);
    if (a->r)
      free(a->r);
    if (a->g)
      free(a->g);
    if (a->b)
      free(a->b);
    a->w = w;
    a->h = h;
    a->d = malloc(sizeof(int)*w*h);
    a->r = malloc(sizeof(int)*w*h);
    a->g = malloc(sizeof(int)*w*h);
    a->b = malloc(sizeof(int)*w*h);
  }
}

void freeMatrix(matrix *a) {
  if (a->d)
    free(a->d);
  if (a->r)
    free(a->r);
  if (a->g)
    free(a->g);
  if (a->b)
    free(a->b);
  a->d = 0;
  a->r = 0;
  a->g = 0;
  a->b = 0;
  a->w = 0;
  a->h = 0;
}

void switchMatrix(matrix *a,matrix *b) {
  int *aux;  
  aux = b->d;
  b->d = a->d;
  a->d = aux;
  aux = b->r;
  b->r = a->r;
  a->r = aux;
  aux = b->g;
  b->g = a->g;
  a->g = aux;
  aux = b->b;
  b->b = a->b;
  a->b = aux;
  int aux2;
  aux2 = b->w;
  b->w = a->w;
  a->w = aux2;
  aux2 = b->h;
  b->h = a->h;
  a->h = aux2;
}

void copyMatrix(matrix *a,matrix *b) {
  if (!a->d) return;
  allocateMatrix(b,a->w,a->h);
  int i;
  int size = a->h*a->w;
  for(i = 0;i < size;++i) {
    b->d[i] = a->d[i];
  }
}
