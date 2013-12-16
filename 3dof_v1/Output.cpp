#include "Output.h"

Image<BWPixel> *Output::newOrReuse(Image<BWPixel> **reuse,int w,int h) const {
  if ((reuse) && (*reuse) && (w==(**reuse).getW() && h==(**reuse).getH()))
    return *reuse;
  else {
    if ((reuse) && (*reuse))
      delete *reuse;
    *reuse = new Image<BWPixel>;
    (**reuse).allocate(w,h);
    return *reuse;
  }
}

VMatrix *Output::newOrReuse(VMatrix **reuse,int w,int h) const {
  if ((reuse) && (*reuse) && (w==(**reuse).getW() && h==(**reuse).getH()))
    return *reuse;
  else {
    if ((reuse) && (*reuse))
      delete *reuse;
    *reuse = new VMatrix;
    (**reuse).allocate(w,h);
    return *reuse;
  }
}

 Image<BWPixel> *Output::procesa(const VMatrix &a,const Image<RGBPixel> &sobela,int wfactor,int hfactor,const Filter &f,int maxdist,double distfactor,double minmovactarea,Image<BWPixel> **reuse1,VMatrix **reuse2) {
  Image<BWPixel> *ret = newOrReuse(reuse1,a.getW(),a.getH());
  VMatrix *ret2 = newOrReuse(reuse2,a.getW(),a.getH());
  (*ret).blank();
  int x,y;
  int val;
  int min,minx,miny;
  int max,maxx,maxy;
  for (int i = 0;i < a.getH();i++) {
    for (int j = 0;j < a.getW();j++) {
      if (a.get(j,i).get()) {
        //Vertical ray
        for (x = 0;x < a.getH();x++) {
          val = abs(x-i);
          if (val<a.getH()/3)
            (*ret).get(j,x).sum(a.get(j,i).get()/(val==0?1:val));
        }
        // Horizontal ray
        for (x = 0;x < a.getW();x++) {
          val = abs(x-j);
          if (val<a.getW()/3)
            (*ret).get(x,i).sum(a.get(j,i).get()/(val==0?1:val));
        }
        // Diagonal RD - LU ray
        x = j+a.getW()/3;
        y = i-a.getW()/3;
        for (;x >= 0 && y < i+a.getW()/3;) {
          if (x >= 0 && x < a.getW() && y >= 0 && y < a.getH())
            (*ret).get(x,y).sum(a.get(j,i).get());
          x--;
          y++;
        }
        // Diagonal LD - RU ray
        x = j-a.getW()/3;
        y = i-a.getW()/3;
        for (;x < j+a.getW()/3 && y < i+a.getW()/3;) {
          if (x >= 0 && x < a.getW() && y >= 0 && y < a.getH())
            (*ret).get(x,y).sum(a.get(j,i).get());
          x++;
          y++;
        }
      }
    }
  }
  (*ret).filter(f,&ret);
  max = 0;
  int mx = 0;
  int my = 0;
  for (int i = 0;i < a.getH();i++) {
    for (int j = 0;j < a.getW();j++) {
      if ((*ret).get(j,i).getV() > max) {
        max = (*ret).get(j,i).getV();
        mx = j*wfactor;
        my = i*hfactor;
      }
    }
  }
  int ox = mx/wfactor;
  int oy = my/hfactor;
  int n = 0;
  (*ret2).blank();

  if (max) {
    valid = 1;
    for (int i = 0;i < a.getH();i++) {
      for (int j = 0;j < a.getW();j++) {
        if (a.get(j,i).get())
          n++;
      }
    }
    valid = 1;
  }
  else {
    valid = 0;
  }

  int dist;
  int distx;
  int disty;
  int iteracion = 0;
  double dists[] = {
0,0.25,0.5,0.75,
1,1.25,1.5,1.75,
2,2.25,2.5,2.75,
3,3.25,3.5,3.75,
4,4.25,4.5,4.75,
5,5.25,5.5,5.75,
6,6.25,6.5,6.75,
7,8,9,10,11,13,16,18,21,24,27,32,37,41,48,50,56,61,72,85,98,113,128,145,162,184,200,244,288};
  double newareascreen = 0;
  while (iteracion < maxdist && min < distfactor*(double)n) {
    min = 0;
    for (int i = 0;i < a.getH();i++) {
      for (int j = 0;j < a.getW();j++) {
        distx = abs(j-ox);
        distx *= distx;
        disty = abs(i-oy);
        disty *= disty;
        dist = distx+disty;
        if (dist<=dists[iteracion]) {
          min++;
          (*ret2).get(j,i) = a.get(j,i);
        }
      }
    }
    newareascreen = dists[iteracion];
    iteracion++;
  }
  movementscreen.x = 0;
  movementscreen.y = 0;
  double up = 0;
  double down = 0;
  double left = 0;
  double right = 0;
  n = 0;
  int nup = 0;
  int ndown = 0;
  int nleft = 0;
  int nright = 0;
  for (int i = 0;i < a.getH();i++) {
    for (int j = 0;j < a.getW();j++) {
      if ((*ret2).get(j,i).getV() != 0 || (*ret2).get(j,i).getH() != 0) {
        movementscreen.x += (*ret2).get(j,i).getH();
        movementscreen.y += (*ret2).get(j,i).getV();
      }
      n++;
    }
  }
  if (n > 0) {
    movementscreen.x *= 4./n;
    movementscreen.y *= 4./n;
  }
  double oldarea = areascreen;
  double movx = movementscreen.x;
  movx = (movx<0?-movx:movx);
  double movy = movementscreen.y;
  movy = (movy<0?-movy:movy);
  if (movx+movy >= minmovactarea) {
    areascreen = newareascreen;
    movementscreen.z = (areascreen-oldarea);
  }
  totalmovement = a.total;
  if (max == 0)
    return ret;
  else {
    positioncenterscreen.x = mx;
    positioncenterscreen.y = my;
  }
  for (int i = 0;i < a.getH();i++) {
    for (int j = 0;j < a.getW();j++) {
      (*ret).get(j,i).setV(((*ret).get(j,i).getV()*255)/max);
    }
  }



  return ret;
}

void Output::soften(const Output &n,double factor,double areafactor,int minimumtotalarea) {
  double dif;
  dif = n.positioncenterscreen.x-positioncenterscreen.x;
  positioncenterscreen.x += factor*dif;
  dif = n.positioncenterscreen.y-positioncenterscreen.y;
  positioncenterscreen.y += factor*dif;
  dif = n.movementscreen.x-movementscreen.x;
  movementscreen.x += factor*dif;
  dif = n.movementscreen.y-movementscreen.y;
  movementscreen.y += factor*dif;
  dif = n.movementscreen.z-movementscreen.z;
  movementscreen.z += factor*dif;
  if (n.totalmovement > minimumtotalarea) {
    targetareascreen = n.areascreen;
  }
  if ((!valid) && (n.valid)) {
    areascreen = targetareascreen;
  }
  if (n.valid) {
    dif = targetareascreen-areascreen;
    areascreen += areafactor*dif;
  }
  
  valid = n.valid;
}
