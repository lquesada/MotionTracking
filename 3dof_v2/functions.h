#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "matrix.h"
#include "point.h"
#include "rectangle.h"
#include <math.h>

void diskFilter(matrix *in,matrix *out);
void sobel(matrix *in,matrix *out);
void normalize(matrix *in,matrix *out);
void normalizePositive(matrix *in,matrix *out);
void medianFilter(matrix *in,int neigh,int min,matrix *out);
void diff(matrix *a,matrix *b,matrix *out);
void absolute(matrix *in,matrix *out);
void square(matrix *in,matrix *out);
void and(matrix *a,matrix *b,matrix *out);
void booleanAnd(matrix *a,matrix *b,matrix *out);
void booleanAndThresholds(matrix *a,int at,matrix *b,int bt,matrix *out);
void normalizeBrightness(matrix *target,matrix *reference,matrix *out);
void average(matrix *in,double *avg);
void sum(matrix *a,int val,matrix *out);
void sumNormalize(matrix *a,int val,matrix *out);
void averageThreshold(matrix *in,int threshold,double *avg);
void averageThresholdCenter(matrix *in,int threshold,rectangle *center,int margin,double *avg);
void averagePosition(matrix *a,point *center);
void averagePositionWeight(matrix *a,point *center);
void radioCollision(matrix *a,point *center,int threshold,rectangle *area,point *ncenter);
void pointDifference(point *a,point *b,int *diff);

#endif
