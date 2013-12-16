#ifndef __RGBPIXEL_H__
#define __RGBPIXEL_H__

#include "Pixel.h"
#include "BWPixel.h"

class RGBPixel : Pixel {

  int r;
  int g;
  int b;

  public:

  inline int getV() const {
    return (r+g+b)/3;
  }

  inline int getR() const {
    return r;
  }

  inline int getG() const {
    return g;
  }

  inline int getB() const {
    return b;
  }

  inline void operator=(const RGBPixel &a) {
    this->r = a.r;
    this->g = a.g;
    this->b = a.b;
  }

  inline void operator=(const BWPixel &a) {
    this->r = a.getV();
    this->g = a.getV();
    this->b = a.getV();
  }

  inline void setRGB(int r,int g,int b) {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  inline void setV(int v) {
    this->r = v;
    this->g = v;
    this->b = v;
  }

  inline void sum(int v) {
    this->r += v;
    this->g += v;
    this->b += v;
  }

};

#endif
