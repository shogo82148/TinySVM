#ifndef _GRAM_MATRIX_H
#define _GRAM_MATRIX_H
#include "kernel.h"
#include "cache.h"

// $Id: gram_matrix.h,v 1.6 2000/12/04 16:41:16 taku-ku Exp $;
class GramMatrix: public Kernel
{
 private:
  double *buf;
  int    *binary_kernel_cache;
  double *(GramMatrix::*_getQ)(const int, const int *);

  double *_getQ_normal        (const int, const int*);
  double *_getQ_binary_char   (const int, const int*);
  double *_getQ_binary_double (const int, const int*);

  feature_node **_x;
  double *_y;

 public:
  Cache <double>        *cache_normal;
  Cache <unsigned char> *cache_binary;

  int size;
  int hit;
  int miss;

  GramMatrix(const BaseExample &, const Param &);
  ~GramMatrix();

  void updateQ(const int, const int *);

  inline double *getQ(const int i, const int *active2index)
  {
    return (this->*_getQ)(i, active2index);
  }

  inline void deleteQ(const int i) 
  {
    if (cache_binary) cache_binary->deleteData(i);
    if (cache_normal) cache_normal->deleteData(i);
  }
};

#endif
