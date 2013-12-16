#include "Filter.h"

Filter::Filter() {
  w = h = 0;
  data = (double *)0;
}

Filter::~Filter() {
  clean();
}

void Filter::allocate(int w,int h) {
  clean();
  this->w = w;
  this->h = h;
  data = new double[w*h];
}

void Filter::clean() {
  if (data)
    delete[] data;
  w = h = 0;
  data = (double *)0;
}

void Filter::blank() {
  for (int i = 0;i < h;i++)
    for (int j = 0;j < w;j++)
      set(j,i,0);
}

void Filter::makeOnes(int size) {
  allocate(size,size);
  double v = 1./(w*h);
  for (int i = 0;i < h;i++)
    for (int j = 0;j < w;j++)
      set(j,i,v);
}

void Filter::makeSobelX() {
  allocate(3,3);
  blank();
  set(0,0,-1);
  set(0,1,-2);
  set(0,2,-1);
  set(2,0,1);
  set(2,1,2);
  set(2,2,1);
}

void Filter::makeSobelY() {
  allocate(3,3);
  blank();
  set(0,0,1);
  set(1,0,2);
  set(2,0,1);
  set(0,2,-1);
  set(1,2,-2);
  set(2,2,-1);
}

