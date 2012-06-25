#ifndef _KERNEL_H
#define _KERNEL_H
#include "misc.h"
#include "param.h"
#include "base_example.h"
#include <math.h>

// $Id: kernel.h,v 1.9 2000/12/01 21:13:52 taku-ku Exp $;
class Kernel
{
protected:
  const int l;
  const int d;
  const int pack_d;
  const feature_node ** x;
  const double *y;

  const int    kernel_type;
  const int    feature_type;
  const int    degree;
  const double param_g;
  const double param_r;
  const double param_s;
  
  double  (Kernel::*_getKernel)(const double) const;

  inline double Kernel::_getKernel_linear(const double _x) const
  {
    return _x;
  }

  inline double Kernel::_getKernel_poly(const double _x) const
  {
    return pow(param_s * _x + param_r, degree);
  }

  inline double dot_normal(const feature_node *x1, const feature_node *x2) const
  {
    register double sum = 0;
    while (x1->index != -1 && x2->index != -1) {
      if (x1->index == x2->index) {
	sum += (x1->value * x2->value);
	++x1; ++x2;
      } else {
	if (x1->index > x2->index) ++x2;
	else ++x1;
      }			
    }
    return sum;
  }

  inline int dot_binary(const feature_node *x1, const feature_node *x2) const
  {
    register int sum = 0;
    while (x1->index != -1 && x2->index != -1) {
      if (x1->index == x2->index) {
	sum++; 
	++x1; ++x2;
      } else {
	if (x1->index > x2->index) ++x2;
	else ++x1;
      }			
    }
    return sum;
  }

public:
  Kernel(const BaseExample& prob, const Param& param)
    :l(prob.l), d(prob.d), pack_d(prob.pack_d), 
    x((const feature_node **)prob.x), y(prob.y),
    kernel_type(param.kernel_type),
    feature_type(prob.feature_type),
    degree(param.degree),
    param_g(param.param_g),
    param_r(param.param_r),
    param_s(param.param_s)
  {
    // default
    switch (kernel_type) {
    case LINEAR:
      _getKernel  = &Kernel::_getKernel_linear;
      break;
    case POLY:
      _getKernel  = &Kernel::_getKernel_poly;
      break;
    default:
      fprintf(stderr,"Kernel::Kernel: Unknown kernel function\n");
      //std::exit(1);
    }
  }

  // wrapper for getKernel
  inline double getKernel(const feature_node *x1, const feature_node *x2)
  {
    return this->getKernel(dot_normal(x1,x2));
  }

  inline double getKernel(const double _x)
  {
    return (this->*_getKernel)(_x);
  }
};

#endif
