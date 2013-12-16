#include "functions.h"
#include "matrix.h"

void diskFilter(matrix *in,matrix *out) {
  allocateMatrix(out,in->w,in->h);
  int i,j;
  for(i = 1;i < in->h-1;++i) {
    for(j = 1;j < in->w-1;++j) {
      out->d[j+i*in->w] =
              0.0251*in->d[j-1+(i-1)*in->w] +	0.1453*in->d[j+(i-1)*in->w] +	0.0251*in->d[j+1+(i-1)*in->w] +	
              0.1453*in->d[j-1+(i)*in->w] +	0.3183*in->d[j+(i)*in->w] +	0.1453*in->d[j+1+(i)*in->w] +	
              0.0251*in->d[j-1+(i+1)*in->w] +	0.1453*in->d[j+(i+1)*in->w] +	0.0251*in->d[j+1+(i+1)*in->w];
    }
  }
  for (j = 0;j < in->w;j++) {
    out->d[j] = out->d[j+in->w];
    out->d[j+(in->h-1)*in->w] = out->d[j+(in->h-2)*in->w];
  }
  for (i = 0;i < in->h;i++) {
    out->d[i*in->w] = out->d[i*in->w+1];
    out->d[i*in->w+in->w-1] = out->d[i*in->w+in->w-2];
  }
}

void sobel(matrix *in,matrix *out) {
  allocateMatrix(out,in->w,in->h);
  int i,j;
  for (j = 0;j < in->w;j++) {
    out->d[j] = 0;
    out->d[j+(in->h-1)*in->w] = 0;
  }
  for (i = 0;i < in->h;i++) {
    out->d[i*in->w] = 0;
    out->d[i*in->w+in->h-1] = 0;
  }
  for(i = 1;i < in->h-1;++i) {
    for(j = 1;j < in->w-1;++j) {
      out->d[j+i*in->w] = (abs(in->d[j-1+(i-1)*in->w] + 2*in->d[j+(i-1)*in->w] + in->d[j+1+(i-1)*in->w] - (in->d[j-1+(i+1)*in->w] + 2*in->d[j+(i+1)*in->w] + in->d[j+1+(i+1)*in->w])) +
                           abs(in->d[j-1+(i-1)*in->w] + 2*in->d[j-1+i*in->w] + in->d[j-1+(i+1)*in->w] - (in->d[j+1+(i-1)*in->w] + 2*in->d[j+1+i*in->w] + in->d[j+1+(i+1)*in->w])));
    }
  }
}

void normalize(matrix *in,matrix *out) {
  allocateMatrix(out,in->w,in->h);
  int i;
  int min = in->d[0];
  int max = in->d[0];
  int val;
  int size = in->h*in->w;
  for(i = 0;i < size;++i) {
    val = in->d[i];
    if (val < min)
      min = val;
    if (val > max)
      max = val;
  }

  double rank = max-min;
  for(i = 0;i < size;++i) {
    out->d[i] = ((in->d[i]-min)*255)/rank;
  }

}

void normalizePositive(matrix *in,matrix *out) {
  allocateMatrix(out,in->w,in->h);
  int i;
  int max = 0;
  int val;
  int size = in->h*in->w;
  for(i = 0;i < size;++i) {
    val = in->d[i];
    if (val > max)
      max = val;
  }

  for(i = 0;i < size;++i) {
    out->d[i] = ((in->d[i])*255)/max;
  }
}

void diff(matrix *a,matrix *b,matrix *out) {
  if (!a->d || !b->d) return;
  allocateMatrix(out,a->w,a->h);
  int i;
  int size = a->h*a->w;
  for(i = 0;i < size;++i) {
    out->d[i] = a->d[i]-b->d[i];
  }
}

void absolute(matrix *in,matrix *out) {
  allocateMatrix(out,in->w,in->h);
  int size = in->h*in->w;
  int i;
  for(i = 0;i < size;++i) {
    if (in->d[i] < 0)
      out->d[i] = -in->d[i];
    else
      out->d[i] = in->d[i];
  }
}
void square(matrix *in,matrix *out) {
  allocateMatrix(out,in->w,in->h);
  int size = in->h*in->w;
  int i;
  for(i = 0;i < size;++i)
    out->d[i] = in->d[i]*in->d[i];
}

void medianFilter(matrix *in,int neigh,int min,matrix *out) {
  allocateMatrix(out,in->w,in->h);
  int i,j;
  int x,y;
  int n;
  for (j = 0;j < in->w;j++) {
    out->d[j] = 0;
    out->d[j+(in->h-1)*in->w] = 0;
  }
  for (i = 0;i < in->h;i++) {
    out->d[i*in->w] = 0;
    out->d[i*in->w+in->h-1] = 0;
  }
  for(i = 0;i < in->h;++i) {
    for(j = 0;j < in->w;++j) {
      n = 
in->d[(i-1)*in->w+j-1] +	in->d[(i-1)*in->w+j]   +	in->d[(i-1)*in->w+j+1] +
in->d[(i)*in->w+j-1]   +	in->d[(i)*in->w+j]     +	in->d[(i)*in->w+j+1]   +
in->d[(i+1)*in->w+j-1] +	in->d[(i+1)*in->w+j]   +	in->d[(i+1)*in->w+j+1];
        if (n/255 >= min)
          out->d[i*in->w+j] = 255;
        else
          out->d[i*in->w+j] = 0;
    }
  }
}

void and(matrix *a,matrix *b,matrix *out) {
  if (!a->d || !b->d) return;
  allocateMatrix(out,a->w,a->h);
  int i;
  int size = a->h*a->w;
  for(i = 0;i < size;++i) {
    out->d[i] = (a->d[i]<b->d[i]?a->d[i]:b->d[i]);
  }
}

void booleanAnd(matrix *a,matrix *b,matrix *out) {
  if (!a->d || !b->d) return;
  allocateMatrix(out,a->w,a->h);
  int i;
  int size = a->h*a->w;
  for(i = 0;i < size;++i) {
    out->d[i] = (a->d[i]&&b->d[i]?255:0);
  }
}

void booleanAndThresholds(matrix *a,int at,matrix *b,int bt,matrix *out) {
  if (!a->d || !b->d) return;
  allocateMatrix(out,a->w,a->h);
  int i;
  int size = a->h*a->w;
  for(i = 0;i < size;++i) {
    out->d[i] = (a->d[i]>=at&&b->d[i]>=at?255:0);
  }
}

void normalizeBrightness(matrix *target,matrix *reference,matrix *out) {
  if (!target->d || !reference->d) return;
  allocateMatrix(out,target->w,target->h);
  double avgref;
  int avgrefi;
  average(reference,&avgref);
  double avgtar;
  int avgtari;
  average(target,&avgtar);
  avgrefi = (int)avgref;
  avgtari = (int)avgtar;
  sumNormalize(target,avgrefi-avgtari,out);
}

void average(matrix *in,double *avg) {
  int i;
  int size = in->h*in->w;
  int sum = 0;
  for(i = 0;i < size;++i) {
    sum += in->d[i];
  }
  *avg = ((double)sum)/((double)size);
}

void sum(matrix *a,int val,matrix *out) {
  allocateMatrix(out,a->w,a->h);
  int i;
  int size = a->h*a->w;
  for(i = 0;i < size;++i) {
    out->d[i] = a->d[i]+val;
  }
}

void sumNormalize(matrix *a,int val,matrix *out) {
  allocateMatrix(out,a->w,a->h);
  int i;
  int size = a->h*a->w;
  int nv;
  for(i = 0;i < size;++i) {
    nv = a->d[i]+val;
    if (nv < 0)
      nv = 0;
    if (nv > 255)
      nv = 255;
    out->d[i] = nv;
  }
}

void averageThreshold(matrix *in,int threshold,double *avg) {
  int i;
  int size = in->h*in->w;
  int sum = 0;
  for (i = 0;i < size;++i) {
    if (in->d[i] >= threshold)
      sum += in->d[i];
  }
  *avg = ((double)sum)/((double)size);
}

void averageThresholdCenter(matrix *in,int threshold,rectangle *center,int margin,double *avg) {
  int i,j;
  int sum = 0;
  int x,y,x2,y2;
  x = center->x-margin; 
  y = center->y-margin; 
  x2 = center->w+2*margin; 
  y2 = center->h+2*margin; 
  if (x < 0)
    x = 0;
  if (y < 0)
    y = 0;
  if (x2 > in->w)
    x2 = in->w;
  if (y2 > in->h)
    y2 = in->h;

  int size = (in->w*in->h);
  for(i = y;i < y2;++i) {
    for(j = x;j < x2;++j) {
      if (in->d[i*in->w+j] >= threshold)
        sum += in->d[i*in->w+j];
    }
  }
  *avg = (double)sum/size;
}


void averagePosition(matrix *a,point *center) {
  int i,j,x,n;
  x = 0;
  n = 0;
  for(i = 0;i < a->h;++i) {
    for(j = 0i;j < a->w;++j) {
      if (a->d[x]) {
        center->x += j;
        center->y += i;
        n++;
      }
      x++;
    }
  } 
  if (n) {
    center->x /= n;
    center->y /= n;
  }
  center->valid = 1;
}

void averagePositionWeight(matrix *a,point *center) {
  int i,j,x,n;
  x = 0;
  n = 0;
  for(i = 0;i < a->h;++i) {
    for(j = 0i;j < a->w;++j) {
      if (a->d[x]) {
        center->x += a->d[x]*j;
        center->y += a->d[x]*i;
        n+=a->d[x];
      }
      x++;
    }
  }
  if (n) {
    center->x /= n;
    center->y /= n;
  }
  center->valid = 1;
}


void radioCollision(matrix *a,point *center,int threshold,rectangle *area,point *ncenter) {
  int rays[32] = {1,1,1,1,1,2,3,4,0,-4,-3,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-3,-4,0,4,3,2,1,1,1,1};
  int cx = 0;
  int cy = 0;
  int nx = 0;
  int ny = 0;
  int x;
  int y;
  int j;
  int xinc;
  int yinc;
  int xinca;
  int yinca;
  int it;
  int cxn = 0,nxn = 0,cyn = 0,nyn = 0;
  int cxp = 0,nxp = 0,cyp = 0,nyp = 0;
  //Lanzamiento de 32 rayos.
  for (j = 0;j < 32;j++) {
    xinc = rays[j];
    yinc = rays[(j+8)%32];
    it = 0;
    xinca = (xinc<0?-xinc:xinc);
    yinca = (yinc<0?-yinc:yinc);
    if (xinc < 0)
      xinc = -1;
    if (xinc > 0)
      xinc = 1;
    if (yinc < 0)
      yinc = -1;
    if (yinc > 0)
      yinc = 1;
    if (xinca == 0) xinca = 9999;
    if (yinca == 0) yinca = 9999;
    x = center->x;
    y = center->y;
    while (a->d[y*a->w+x] < threshold && y >= 0 && y < a->h && x >= 0 && x < a->w) {
      if (it%xinca == 0)
        x+=xinc;
      if (it%yinca == 0)
        y+=yinc;
      it++;
    }
    if (x < 0)
      x = 0;
    if (x >= a->w)
      x = a->w;
    if (y < 0)
      y = 0;
    if (y >= a->h)
      y = a->h;
    if (xinc) {
      cx+=x-center->x;
      nx++;
      if (xinc < 0) {
        cxn+=x-center->x;
        nxn++;
      }
      if (xinc > 0) {
        cxp+=x-center->x;
        nxp++;
      }
    }
    if (yinc) {
      cy+=y-center->y;
      ny++;
      if (yinc < 0) {
        cyn+=y-center->y;
        nyn++;
      }
      if (yinc > 0) {
        cyp+=y-center->y;
        nyp++;
      }
    }
    area->px[j] = x;
    area->py[j] = y;
  }
  if (nx)
    cx/=nx;
  if (ny)
    cy/=ny;
  if (nxn)
    cxn/=nxn;
  if (nyn)
    cyn/=nyn;
  if (nxp)
    cxp/=nxp;
  if (nyp)
    cyp/=nyp;

  area->x = center->x+cxn;
  area->y = center->y+cyn;
  area->w = cxp-cxn;
  area->h = cyp-cyn;
  area->valid = center->valid;
  area->a = area->w*area->h;
  area->ar = sqrt(area->w*area->h);
  printf("%d %d %f\n",area->a,area->ar,(double)log((double)area->a));

  ncenter->x = center->x+cx;
  if (ncenter->x < 0)
    ncenter->x = 0;
  if (ncenter->x >= a->w)
    ncenter->x = a->w;
  ncenter->y = center->y+cy;
  if (ncenter->y < 0)
    ncenter->y = 0;
  if (ncenter->y >= a->h)
    ncenter->y = a->h;
  ncenter->valid = center->valid;
}

void pointDifference(point *a,point *b,int *diff) {
  *diff = abs(a->x-b->x)+abs(a->y-b->y);
}

