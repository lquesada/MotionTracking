#include "VMatrix.h"

//  int w;
//  int h;
//  Vector *data;

VMatrix *VMatrix::newOrReuse(VMatrix **reuse,int w,int h) const {
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

Image<BWPixel> *VMatrix::newOrReuse(Image<BWPixel> **reuse,int w,int h) const {
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

Image<RGBPixel> *VMatrix::newOrReuse(Image<RGBPixel> **reuse,int w,int h) const {
  if ((reuse) && (*reuse) && (w==(**reuse).getW() && h==(**reuse).getH()))
    return *reuse;
  else {
    if ((reuse) && (*reuse))
      delete *reuse;
    *reuse = new Image<RGBPixel>;
    (**reuse).allocate(w,h);
    return *reuse;
  }
}

VMatrix::VMatrix() {
  w = h = 0;
  data = (Vector *)0;
}

VMatrix::~VMatrix() {
  clean();
}

VMatrix::VMatrix(const VMatrix &a) {
  data = 0;
  *this = a;
}

VMatrix &VMatrix::operator=(const VMatrix &a) {
  clean();
  w = a.getW();
  h = a.getH();
  data = new Vector[w*h];
  for (int i = 0;i < h;i++)
    for (int j = 0;j < w;j++)
      get(j,i) = a.get(j,i);
  return *this;
}

void VMatrix::allocate(int w,int h) {
  clean();
  this->w = w;
  this->h = h;
  data = new Vector[w*h];
}

void VMatrix::clean() {
  if (data)
    delete[] data;
  w = h = 0;
  data = (Vector *)0;
}

void VMatrix::blank() {
  for (int i = 0;i < h;i++) {
    for (int j = 0;j < w;j++) {
      get(j,i).setH(0);
      get(j,i).setV(0);
    }
  }
}

Image<BWPixel> *VMatrix::getImage(Image <BWPixel> **reuse) const {
  Image<BWPixel> *ret = newOrReuse(reuse,w,h);
  int max = 0;
  for (int i = 0;i < h;i++) {
    for (int j = 0;j < w;j++) {
      (*ret).get(j,i).setV(get(j,i).get());
      if ((*ret).get(j,i).getV() > max)
        max = (*ret).get(j,i).getV();

    }
  }
  if (max)
    for (int i = 0;i < h;i++) {
      for (int j = 0;j < w;j++) {
        (*ret).get(j,i).setV((*ret).get(j,i).getV()*255/max);
      }
    }
  return ret;
}

VMatrix *VMatrix::greaterThanZeroThresholdFilter(const Image<RGBPixel> &a,VMatrix **reuse) const {
  VMatrix *ret = newOrReuse(reuse,getW(),getH());
    for (int i = 0;i < h;i++) {
      for (int j = 0;j < w;j++) {
        if (a.get(j,i).getV() > 0) {
          (*ret).get(j,i).setV(get(j,i).getV());
          (*ret).get(j,i).setH(get(j,i).getH());
        }
        else {
          (*ret).get(j,i).setV(0);
          (*ret).get(j,i).setH(0);
        }
      }
    }
    return ret;
}

int comparapixel(const RGBPixel &a,const RGBPixel &b) {
  int val = (abs(a.getR()-b.getR()) + abs(a.getG()-b.getG()) + abs(a.getB()-b.getB()));
  return val;
}

VMatrix *VMatrix::compare(const Image<RGBPixel> &a,const Image<RGBPixel> &b,int blockw,int blockh,int maxneighborhood,int overloadmovement,VMatrix **reuse) const {
  int nblocksw = a.getW()/blockw;
  int nblocksh = a.getH()/blockh;
  VMatrix *ret = newOrReuse(reuse,nblocksw,nblocksh);
  ret->total = 0;
  for (int i = 0;i < nblocksw;i++) {
    (*ret).get(i,0).setV(0);
    (*ret).get(i,nblocksh-1).setV(0);
    (*ret).get(i,0).setH(0);
    (*ret).get(i,nblocksh-1).setH(0);
  }
  for (int j = 0;j < nblocksh;j++) {
    (*ret).get(0,j).setV(0);
    (*ret).get(nblocksw-1,j).setV(0);
    (*ret).get(0,j).setH(0);
    (*ret).get(nblocksw-1,j).setH(0);
  }
  int iniblockw;
  int finblockw;
  int iniblockh;
  int finblockh;
  int centrow;
  int centroh;
  int best,besti,bestj;
  int range = 255/((maxneighborhood*2)+1);
  double mult = 254./range;
  for (int bi = 1;bi < nblocksh-1;bi++) {
    for (int bj = 1;bj < nblocksw-1;bj++) {
      iniblockw = bj*blockw;
      finblockw = (bj+1)*blockw;
      iniblockh = bi*blockh;
      finblockh = (bi+1)*blockh;
      centrow = iniblockw+blockw/2;
      centroh = iniblockh+blockh/2;
      best = 9999999;
      besti = 0;
      bestj = 0;

      for (int di = -maxneighborhood;di < maxneighborhood-1;di++) {
        for (int dj = -maxneighborhood;dj < maxneighborhood-1;dj++) {
          if (iniblockw+dj >= 0 && iniblockw+dj < a.getW() && iniblockh+di >= 0 && iniblockh+di < a.getH()) {
            int sum = 0;
            for (int i = iniblockh;i < finblockh;i++)
              for (int j = iniblockw;j < finblockw;j++)
                sum += comparapixel(a.get(j,i),b.get(j+dj,i+di));
            sum += overloadmovement*(abs(di)+abs(dj));
            if (sum < best) {
              best = sum;
              besti = di;
              bestj = dj;
            }
          }
        }
      }
      (*ret).get(bj,bi).setV(((double)bestj)*((double)range));
      (*ret).get(bj,bi).setH(((double)besti)*((double)range));
      ret->total += abs(((double)bestj)*((double)range))+abs(((double)besti)*((double)range));
    }
  }
  return ret;
}

VMatrix *VMatrix::getVMatrix(const Image<RGBPixel> &a,VMatrix **reuse) const {
  VMatrix *ret = newOrReuse(reuse,a.getW(),a.getH());
    for (int i = 0;i < h;i++) {
      for (int j = 0;j < w;j++) {
        (*ret).get(j,i).setH(a.get(j,i).getR()-128);
        (*ret).get(j,i).setV(a.get(j,i).getG()-128);
      }
    }
    return ret;
}

