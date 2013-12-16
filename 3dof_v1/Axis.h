#ifndef __AXIS_H__
#define __AXIS_H__

#include "Output.h"

class Axis {

  public:

  double x;
  double y;
  double z;

  double speedx;
  double speedy;
  double speedz;

  Axis();  
  void update(const Output &o);
};

#endif
