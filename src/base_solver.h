#ifndef _SOLVER_H
#define _SOLVER_H
#include "model.h"

// $Id: base_solver.h,v 1.3 2000/11/13 01:00:22 taku-ku Exp $;

class BaseSolver
{
 public:
  virtual Model *learn()  = 0;
  virtual int writeSnapShot(const char *) = 0;
  virtual int readSnapShot(const char *) = 0;
  virtual ~BaseSolver() {};
};
#endif
