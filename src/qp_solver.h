#ifndef _QP_SOLVER_H
#define _QP_SOLVER_H
#include "cache.h"
#include "param.h"
#include "base_example.h"
#include "q_matrix.h"

// $Id: qp_solver.h,v 1.6 2001/01/16 19:37:20 taku-ku Exp $;

#define LOWER_BOUND (-1)
#define UPPER_BOUND (+1)
#define FREE        (0)

// min (b^T \alpha + 0.5(\alpha^T Q \alpha))
//   0 <= alpha_i <= C
//   y^T \alpha = \delta
//   y_i = +1 or -1

namespace TinySVM {

class QP_Solver
{
private:
  int           l;             // # of training examples
  double        C;             // slack valiable
  double        eps;           // stopping criterion
  int           shrink_size;   // number of iterations a variable needs for shrinking;
  int           final_check;   // do final check or not
  Param         param;         // global parameter
  int           active_size;   // # of active size
  feature_node  **x;
  double        *y;            // label of training data
  double        *b;            // linar constraints vector
  double        *G;	       // gradient of objective function
  double        *alpha;        // alpha themselves
  int           *status;       // LOWER_BOUND, UPPER_BOUND, FREE
  int           *shrink_iter;  // # of iterations of lower(upper) bound
  int           *active2index; // active -> index  table
  int           active_num;    // # of active (working) set
  double        lambda_eq;     // threshold b;
  double        shrink_eps;    // shirinking threshold 
  QMatrix       *q_matrix;  // gram_matrix;
  int           iter;          // number of iteration;
  int           hit_old;       //

  inline void swap_index     (const int, const int);
  inline int  is_upper_bound (const int i) const { return status[i] == UPPER_BOUND; }
  inline int  is_lower_bound (const int i) const { return status[i] == LOWER_BOUND; }
  inline int  is_free        (const int i) const { return status[i] == FREE;        }

  inline int alpha2status (const double alpha) const
  {
    if (alpha >= C - EPS_A)  return UPPER_BOUND;
    else if (alpha <= EPS_A) return LOWER_BOUND;
    else                     return FREE;
  }
  
  void learn_sub();
  int  check_inactive();

public:
  int solve(const BaseExample &, const Param &, double *, double *, double *, 
	    double &, double &);

};


};
#endif

