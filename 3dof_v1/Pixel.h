#ifndef __PIXEL_H__
#define __PIXEL_H__

class Pixel {
  public:

  inline int getV() const;
  inline int getR() const;
  inline int getG() const;
  inline int getB() const;
  inline void operator=(const Pixel &a);
  inline void setRGB(int r,int g,int b);
  inline void setV(int v);

};

#endif
