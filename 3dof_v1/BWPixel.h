#ifndef __BWPIXEL_H__
#define __BWPIXEL_H__

#include "Pixel.h"

class BWPixel : Pixel {

  int v;

  public:

  inline int getV() const {
    return v;
  }

  inline int getR() const {
    return v;
  }
  
  inline int getG() const {
    return v;
  }

  inline int getB() const {
    return v;
  }

  inline void operator=(const BWPixel &a) {
    this->v = a.v;
  }

  inline void setRGB(int r,int g,int b) {
    this->v = (r+g+b)/3;
  }

  inline void setV(int v) {
    this->v = v;
  }

  inline void sum(int v) {
    this->v += v;
  }

};

#endif
