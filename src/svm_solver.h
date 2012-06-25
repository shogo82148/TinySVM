#ifndef _SVM_SOLVER_H
#define _SVM_SOLVER_H
#include "cache.h"
#include "param.h"
#include "base_example.h"
#include "base_solver.h"

// $Id: svm_solver.h,v 1.11 2001/01/16 19:37:20 taku-ku Exp $;

namespace TinySVM {

class SVM_Solver: public BaseSolver
{
public:
  SVM_Solver(const BaseExample &e, const Param &p): BaseSolver(e,p) {};
  ~SVM_Solver() {};

  Model *learn();
};


};
#endif

