#ifndef _Q_MATRIX_H
#define _Q_MATRIX_H
#include "kernel.h"
#include "cache.h"

// $Id: q_matrix.h,v 1.4 2001/08/22 14:11:49 taku-ku Exp $;
namespace TinySVM {

class QMatrix: public Kernel
{
 private:
  double *buf;
  int    *binary_kernel_cache;
  double *(QMatrix::*_getQ)(const int, const int);

  double *_getQ_normal        (const int, const int);
  double *_getQ_binary_char   (const int, const int);
  double *_getQ_binary_double (const int, const int);

  Cache <double>        *cache_normal;
  Cache <unsigned char> *cache_binary;

 public:
  int size;
  int hit;
  int miss;
  double cache_size;

  QMatrix(const BaseExample &, const Param &);
  ~QMatrix();

  // misc function
  void update (const int);
  void delete_index (const int);
  void swap_index (const int, const int);
  void rebuildCache(int);
  void set (double *_y, feature_node **_x) { y = _y; x = _x; };

  // main
  inline double *getQ(const int i, const int active_size)
  {
    return (this->*_getQ)(i, active_size);
  }
};


};
#endif

