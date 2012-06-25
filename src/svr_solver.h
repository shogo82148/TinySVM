#ifndef _SVR_SOLVER_H
#define _SVR_SOLVER_H
#include "cache.h"
#include "param.h"
#include "base_example.h"
#include "base_solver.h"

// $Id: svr_solver.h,v 1.2 2001/01/16 19:37:20 taku-ku Exp $;

namespace TinySVM {

class SVR_Solver: public BaseSolver
{
public:
  SVR_Solver(const BaseExample &e, const Param &p): BaseSolver(e,p) {};
  ~SVR_Solver() {};

  Model *learn();
};


};
#endif

