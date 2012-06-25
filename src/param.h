#ifndef _PARAM_H
#define _PARAM_H

class Param
{
public:
  int    kernel_type;
  int    feature_type;
  int    solver_type;
  int    degree;
  double param_g;
  double param_r;
  double param_s;
  double cache_size; 
  double C;
  double eps;
  int    verbose;
  int    shrink_size;
  double shrink_th;
  int    final_check;

  int set (int,  char **);
  int set (const char *);

  Param();
};

#endif
