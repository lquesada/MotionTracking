#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdlib.h>

class Vector {
  double hor;
  double ver;

  public:
  inline double getH() const { return hor; }
  inline double getV() const { return ver; }
  inline double get() const { return abs(ver+hor); }
  inline void setH(double a) { hor = a; }
  inline void setV(double a) { ver = a; }

};

#endif
