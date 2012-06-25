#ifndef _CLASSIFIER_H
#define _CLASSIFIER_H
#include "kernel.h"

// $Id: classifier.h,v 1.5 2001/08/24 13:07:52 taku-ku Exp $;
namespace TinySVM {

class Classifier: public Kernel
{
 private:
  double model_bias;
  int    *dot_buf;
  double **binary_kernel_cache;
  int    **fi2si;

  double  (Classifier::*_getDistance) (const feature_node *) const;
  double  _getDistance_binary (const feature_node *) const;
  double  _getDistance_normal (const feature_node *) const;

 public:
  Classifier (const BaseExample &, const Param &);
  ~Classifier ();

  inline double getDistance (const char *s)
  {
    feature_node *node = str2feature_node (s);
    double d = getDistance(node);
    delete [] node;
    return d;
  }
  
  inline double getDistance (const feature_node *_x) 
  {
    return (this->*_getDistance)(_x);
  }
};
};
#endif

