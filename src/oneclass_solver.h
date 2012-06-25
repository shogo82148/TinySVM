#ifndef _ONECLASS_SOLVER_H
#define _ONCLASSL_SOLVER_H
#include "cache.h"
#include "param.h"
#include "base_example.h"
#include "base_solver.h"

// $Id: oneclass_solver.h,v 1.1 2001/12/06 12:58:54 taku-ku Exp $;

namespace TinySVM {

class OneClass_Solver: public BaseSolver
{
public:
  OneClass_Solver(const BaseExample &e, const Param &p): BaseSolver(e,p) {};
  ~OneClass_Solver() {};

  Model *learn();
};


};
#endif

