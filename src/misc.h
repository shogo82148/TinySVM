#ifndef _MISC_H
#define _MISC_H

namespace TinySVM {
   
enum { LINEAR, POLY, NEURAL, RBF, ANOVA };
enum { DOUBLE_FEATURE, BINARY_FEATURE };
enum { SVM, SVR, ONE_CLASS };

struct feature_node
{
  int index;
  double value;
};
};
#endif

