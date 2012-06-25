#ifndef _SOLVER_H
#define _SOLVER_H
#include "model.h"

// $Id: base_solver.h,v 1.6 2001/01/16 19:37:20 taku-ku Exp $;

namespace TinySVM {

class BaseSolver
{
 protected:
  const  BaseExample   &example;
  const  Param         param; 
  const  int           l;

 public:
  virtual Model *learn()  = 0;
  BaseSolver(const BaseExample &e, const Param &p): 
    example (e), param (p), l (e.l) {};
  virtual ~BaseSolver() {};
};

};
#endif

