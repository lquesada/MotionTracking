#ifndef __VMatrix_H__
#define __VMatrix_H__

template <class T> class Image;
#include "Image.template"
#include "RGBPixel.h"
#include "Vector.h"
#include <stdlib.h>

class VMatrix {

  int w;
  int h;
  Vector *data;

  VMatrix *newOrReuse(VMatrix **reuse,int w,int h) const;
  Image<RGBPixel> *newOrReuse(Image<RGBPixel> **reuse,int w,int h) const;
  Image<BWPixel> *newOrReuse(Image<BWPixel> **reuse,int w,int h) const;

  public:

  int total;

  VMatrix();
  ~VMatrix();
  VMatrix(const VMatrix &a);
  VMatrix &operator=(const VMatrix &a);

  void allocate(int w,int h);
  void clean();
  void blank();

  Image<BWPixel> *getImage(Image <BWPixel> **reuse) const;
  VMatrix *greaterThanZeroThresholdFilter(const Image<RGBPixel> &a,VMatrix **reuse) const;
  VMatrix *compare(const Image<RGBPixel> &a,const Image<RGBPixel> &b,int blockw,int blockh,int maxneighborhood,int overloadmovement,VMatrix **reuse) const;
  VMatrix *getVMatrix(const Image<RGBPixel> &a,VMatrix **reuse) const;

  inline Vector &get(int x,int y) { return data[x+y*w]; }
  inline const Vector &get(int x,int y) const { return data[x+y*w]; }
  inline int getW() const { return w; }
  inline int getH() const { return h; }

};

#endif
