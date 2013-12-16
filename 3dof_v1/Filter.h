#ifndef __FILTER_H__
#define __FILTER_H__

class Filter {

  int w;
  int h; 
  double *data;

  public:

  Filter();
  ~Filter();
  void allocate(int w,int h);
  void clean();
  void blank();
  void makeOnes(int size);
  void makeSobelX();
  void makeSobelY();

  inline double get(int x,int y) const { return data[x+y*w]; }
  inline void set(int x,int y,double v) { data[x+y*w] = v; }

  inline int getW() const { return w; }

  inline int getH() const { return h; }

};

#endif
