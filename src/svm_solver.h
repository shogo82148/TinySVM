#ifndef _SVM_SOLVER_H
#define _SVM_SOLVER_H
#include "cache.h"
#include "param.h"
#include "base_example.h"
#include "base_solver.h"
#include "gram_matrix.h"

// $Id: svm_solver.h,v 1.8 2000/12/01 21:13:52 taku-ku Exp $;

// SVM_Solver // SMO+SVMlight algorithm, inherited from libsvm
#define LOWER_BOUND (-1)
#define UPPER_BOUND (+1)
#define FREE    (0)

class SVM_Solver: public BaseSolver
{
private:
  // global parameter

  const  BaseExample   &example;
  const  Param         param;         // Should it be Reference???
  const  int           l;	      // # of training examples
  const  feature_node  **x;           // training vectors themselves
  const  double        *y;            // label of training data
  const  double        C;             // slack valiable
  const  double        eps;	      // stopping criterion
  const  int           shrink_size;   // number of iterations a variable needs for shrinking;
  const  double        shrink_th;
  const  int           final_check;

  // valiable using for optimization
  double *G;	               // gradient of objective function
  double *alpha;               // alpha themselves
  int    *alpha_status;        // LOWER_BOUND, UPPER_BOUND, FREE
  int    *shrink_iter;         // # of iterations of lower(upper) bound
  int    *active2index;        // active -> index  table
  int    *index2active;        // index  -> active table
  int     active_num;          // # of active (working) set
  double  lambda_eq;
  double  shrink_eps;
  int     iter;
  int     hit_old;

  // Gram Matrix
  GramMatrix *gram_matrix;

  // misc function
  inline int is_upper_bound(const int i) const { return alpha_status[i] == UPPER_BOUND; }
  inline int is_lower_bound(const int i) const { return alpha_status[i] == LOWER_BOUND; }
  inline int is_free(const int i) const        { return alpha_status[i] == FREE; }
  inline int is_active(const int i) const      { return index2active[i] >=  0; };
  inline int is_inactive(const int i) const    { return index2active[i] == -1; };

  // add i-th data to active data set
  inline void activate(int i) 
  {
    if (is_active(i)) return;
    active2index[active_num] = i;
    shrink_iter[i] = 0;
    index2active[i] = active_num;
    active_num++;
    active2index[active_num] = -1;
  }

  // remove i-th data from active data set
  inline void inactivate(int i)
  {
    if (is_inactive(i)) return;
    shrink_iter[i] = 0;
    active_num--;
    int p = index2active[i];     // swap last node <-> i
    index2active[active2index[active_num]] = p;
    active2index[p] = active2index[active_num];
    index2active[i] = -1;
    active2index[active_num] = -1;
  }

  void learn_sub();
  int  check_inactive();

public:
  SVM_Solver(const BaseExample&, const Param);
  ~SVM_Solver();

  // learn
  Model *learn();

  int readSnapShot(const char *);
  int writeSnapShot(const char *);
};

#endif
