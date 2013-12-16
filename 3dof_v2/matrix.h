#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct matrix {
  int *r;
  int *g;
  int *b;
  int *d;
  int w;
  int h;
} matrix;

void allocateMatrix(matrix *a,int w,int h);
void freeMatrix(matrix *a);
void switchMatrix(matrix *a,matrix *b);
void copyMatrix(matrix *a,matrix *b);

#endif
