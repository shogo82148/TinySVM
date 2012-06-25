#include "svm_solver.h"
#include "common.h"
#include "example.h"
#include "classifier.h"
#include "timer.h"

// $Id: svm_solver.cc,v 1.18 2000/12/08 15:01:02 taku-ku Exp $;

// Original Solver stems from libsvm 2.0 package 
SVM_Solver::SVM_Solver (const BaseExample & e, const Param p) :
example (e), param (p), l (e.l), x ((const feature_node **) e.x), y (e.y),
C (p.C), eps (p.eps), shrink_size (p.shrink_size), shrink_th (p.shrink_th),
final_check (p.final_check)
{
}

SVM_Solver::~SVM_Solver ()
{
  if (G)            delete [] G;
  if (alpha)        delete [] alpha;
  if (shrink_iter)  delete [] shrink_iter;
  if (alpha_status) delete [] alpha_status;
  if (active2index) delete [] active2index;
  if (index2active) delete [] index2active;
}

void
SVM_Solver::learn_sub ()
{
  // memory allocation
  try {
    active2index = new int[l + 1];
    index2active = new int[l + 1];
  }

  catch (...) {
    fprintf (stderr, "SVM_Solver::learn_sub(): Out of memory\n");
    exit (EXIT_FAILURE);
  }

  // initialize
  active_num = 0;
  for (int i = 0; i < l; i++) {
    index2active[i] = -1;
    activate (i);
  }

  fprintf (stdout, "%6d examples, cache size: %d\n",
	   active_num, gram_matrix->size);

  /////////////////////////////////////////////////////////
  //
  // learning step, I know it is too complicated...
  //
  for (int k;; iter++) {

    /////////////////////////////////////////////////////////
    //
    // Select Working set
    //
    double Gmax1 = -INF;
    int i = -1;
    double Gmax2 = -INF;
    int j = -1;

    for (int n = 0; (k = active2index[n]) != -1; n++) {
      if (y[k] > 0) {
	if (!is_upper_bound (k)) {
	  if (-G[k] > Gmax1) {
	    Gmax1 = -G[k];
	    i = k;
	  }
	}

	if (!is_lower_bound (k)) {
	  if (G[k] > Gmax2) {
	    Gmax2 = G[k];
	    j = k;
	  }
	}
      } else {
	if (!is_upper_bound (k)) {
	  if (-G[k] > Gmax2) {
	    Gmax2 = -G[k];
	    j = k;
	  }
	}

	if (!is_lower_bound (k)) {
	  if (G[k] > Gmax1) {
	    Gmax1 = G[k];
	    i = k;
	  }
	}
      }
    }

    /////////////////////////////////////////////////////////
    //
    // Solving QP sub problems
    //
    double old_alpha_i = alpha[i];
    double old_alpha_j = alpha[j];

    double *Q_i = gram_matrix->getQ (i, active2index);
    double *Q_j = gram_matrix->getQ (j, active2index);

    if (y[i] * y[j] < 0) {
      double L = max (0.0, alpha[j] - alpha[i]);
      double H = min (C, C + alpha[j] - alpha[i]);
      alpha[j] += (-G[i] - G[j]) / (Q_i[i] + Q_j[j] + 2 * Q_i[j]);
      if (alpha[j] >= H)      alpha[j] = H;
      else if (alpha[j] <= L) alpha[j] = L;
      alpha[i] += (alpha[j] - old_alpha_j);
    } else {
      double L = max (0.0, alpha[i] + alpha[j] - C);
      double H = min (C, alpha[i] + alpha[j]);
      alpha[j] += (G[i] - G[j]) / (Q_i[i] + Q_j[j] - 2 * Q_i[j]);
      if (alpha[j] >= H)      alpha[j] = H;
      else if (alpha[j] <= L) alpha[j] = L;
      alpha[i] -= (alpha[j] - old_alpha_j);
    }

    /////////////////////////////////////////////////////////
    //
    // update alpha_status
    // 
    if (alpha[i] >= C - EPS_A)
      alpha_status[i] = UPPER_BOUND;
    else if (alpha[i] <= EPS_A)
      alpha_status[i] = LOWER_BOUND;
    else
      alpha_status[i] = FREE;

    if (alpha[j] >= C - EPS_A)
      alpha_status[j] = UPPER_BOUND;
    else if (alpha[j] <= EPS_A)
      alpha_status[j] = LOWER_BOUND;
    else
      alpha_status[j] = FREE;

    double delta_alpha_i = alpha[i] - old_alpha_i;
    double delta_alpha_j = alpha[j] - old_alpha_j;

    /////////////////////////////////////////////////////////
    //
    // Update O and Calculate \lambda^eq for shrinking, Calculate lambda^eq,
    // (c.f. Advances in Kernel Method pp.175)
    // lambda_eq = 1/|FREE| \sum_{i \in FREE} y_i - \sum_{l} y_i \alpha_i k(x_i,x_j) (11.29)
    //
    lambda_eq = 0.0;
    int size_A = 0;
    for (int n = 0; (k = active2index[n]) != -1; n++) {
      G[k] += Q_i[k] * delta_alpha_i + Q_j[k] * delta_alpha_j;
      if (is_free (k)) {
	lambda_eq -= G[k] * y[k];
	size_A++;
      }
    }

    /////////////////////////////////////////////////////////
    //
    // Select example for shrinking,
    // (c.f. 11.5 Efficient Implementation in Advances in Kernel Method pp. 175)
    //
    lambda_eq = size_A ? (lambda_eq / size_A) : 0.0;
    double kkt_violation = 0.0;

    for (int n = 0; (k = active2index[n]) != -1;) {
      double lambda_up = -(G[k] + y[k] * lambda_eq);	// lambda_lo = -lambda_up

      // termination criteria (11.32,11.33,11.34)
      if (!is_lower_bound (k) && lambda_up < -kkt_violation) kkt_violation = -lambda_up;
      if (!is_upper_bound (k) && lambda_up >  kkt_violation) kkt_violation =  lambda_up;

      // "If the estimate (11.30) or (11.31) was positive (or above some threshold) at
      // each of the last h iterations, it is likely that this will be true at the  optimal solution" 
      // lambda^up  (11.30) lambda^low = lambda^up * alpha_status[k]
      //      if (lambda_up * alpha_status[k] > 0) {
      if (lambda_up * alpha_status[k] > shrink_eps) {
	if (shrink_iter[k]++ >= shrink_size) {
	  inactivate (k);	// remove this data from working set
	  gram_matrix->deleteQ (k);
	  continue;
	}
      } else {
	// reset iter, if current data dose not satisfy the condition (11.30), (11.31)
	shrink_iter[k] = 0;
      }
      n++;			// if current data is not shrinked, incriment index for working sets
    }

    /////////////////////////////////////////////////////////
    //
    // Check terminal criteria, show information of iteration
    //
    if (kkt_violation < eps)
      return;

    if ((iter % 50) == 0) {
      fprintf (stdout, ".");
      fflush (stdout);
    };

    if ((iter % 1000) == 0) {
      fprintf (stdout, " %6d %6d %1.4f %4d/%5.1f%% %6d/%5.1f%%",
	       iter, active_num, kkt_violation,
	       gram_matrix->hit - hit_old,
	       100.0 * (gram_matrix->hit - hit_old) / 2000,
	       gram_matrix->hit,
	       100.0 * gram_matrix->hit / (gram_matrix->hit +
					   gram_matrix->miss));
      fflush (stdout);

      hit_old = gram_matrix->hit;

      // This shrink eps rule is delivered from svm_light.
      shrink_eps = shrink_eps * 0.7 + kkt_violation * 0.3;

      // recursive learning
      if (1.0 * active_num / l >= shrink_th || gram_matrix->size >= l) {
	fprintf (stdout, "\n");
	fflush (stdout);
      } else {
	try {
	  gram_matrix->updateQ (active_num, active2index);

	  Example *recursive_example = new Example ();
	  recursive_example->ref ();	// important

	  for (int n = 0; (k = active2index[n]) != -1; n++)
	    recursive_example->add ((const double) y[k], (feature_node *) x[k]);

	  SVM_Solver *recursive_solver =  new SVM_Solver (*recursive_example, param);
	  int *recursive2current         = new int    [active_num];
	  recursive_solver->alpha        = new double [active_num];
	  recursive_solver->alpha_status = new int    [active_num];
	  recursive_solver->G            = new double [active_num];
	  recursive_solver->shrink_iter  = new int    [active_num];
	  recursive_solver->shrink_eps   = shrink_eps;
	  recursive_solver->iter         = iter + 1;
	  recursive_solver->gram_matrix  = gram_matrix;
	  recursive_solver->hit_old      = gram_matrix->hit;

	  for (int n = 0; (k = active2index[n]) != -1; n++) {
	    recursive2current[n] = k;
	    recursive_solver->alpha[n] = alpha[k];
	    recursive_solver->alpha_status[n] = alpha_status[k];
	    recursive_solver->G[n] = G[k];
	    recursive_solver->shrink_iter[n] = shrink_iter[k];
	  }

	  // learn
	  fprintf (stdout, " shrinking ...\n\n");
	  fflush (stdout);
	  recursive_solver->learn_sub ();

	  lambda_eq = recursive_solver->lambda_eq;
	  iter = recursive_solver->iter;

	  for (int n = 0; n < recursive_solver->l; n++) {
	    int k = recursive2current[n];
	    alpha[k] = recursive_solver->alpha[n];
	    alpha_status[k] = recursive_solver->alpha_status[n];
	    G[k] = recursive_solver->G[n];
	    shrink_iter[k] = recursive_solver->shrink_iter[n];
	    if (recursive_solver->is_inactive (n)) inactivate (k);
	  }

	  delete [] recursive2current;
	  delete    recursive_solver;
	  delete    recursive_example;
	}

	catch (...) {
	  fprintf (stderr, "SVM_Solver::learn_sub(): Out of memory\n");
	  exit (EXIT_FAILURE);
	}

	return;
      }
    }
  }

  return;
}

int
SVM_Solver::check_inactive ()
{
  // final check
  fprintf (stdout, "\nChecking optimality of inactive variables ");
  fflush (stdout);

  // make dummy classifier
  Model *tmp_model = new Model (param);
  tmp_model->ref ();
  tmp_model->b = -lambda_eq;
  for (int i = 0; i < l; i++) {
    if (!is_lower_bound (i))
      tmp_model->add (alpha[i] * y[i], (feature_node *) x[i]);
  }

  int react_num = 0;
  for (int k = 0; k < l; k++) {
    if (is_active (k))
      continue;			// check inactiave data only
    double lambda_up = 1 - y[k] * tmp_model->classify (x[k]);

    // Oops!, k-th data must be added to the active example.
    if ((!is_lower_bound (k) && lambda_up < -eps) ||
	(!is_upper_bound (k) && lambda_up > eps)) {
      activate (k);
      react_num++;
    }
  }

  delete tmp_model;
  shrink_eps = 2.0;		// reset shrink eps

  fprintf (stdout, " re-activated: %d\n", react_num);
  return react_num;
}

Model *
SVM_Solver::learn ()
{
  // initialize
  Timer timer;

  try {
    gram_matrix = new GramMatrix (example, param);
    shrink_iter = new int[l];
    alpha = new double[l];
    alpha_status = new int[l];
    G = new double[l];
    shrink_eps = 2.0;
    iter = 1;
    active_num = 0;
    hit_old = 0;

    for (int i = 0; i < l; i++) {
      G[i] = -1;
      alpha[i] = 0;
      alpha_status[i] = LOWER_BOUND;
      shrink_iter[i] = 0;
    }
  }

  catch (...) {
    fprintf (stderr, "SVM_Solver::learn(): Out of memory\n");
    exit (EXIT_FAILURE);
  }

  for (;;) {
    learn_sub ();		// learn (recursive call)
    if (final_check == 0 || check_inactive () == 0)  break; // check all inactive data
    delete gram_matrix;
    delete [] active2index;
    delete [] index2active;
    gram_matrix = new GramMatrix (example, param);
  }

  Model *out_model = new Model (param);	// make output model
  out_model->b = -lambda_eq;	// NOTE: the equivalence of lambda_eq  and b in (1.18) pp.174 
  out_model->svindex = new int[l];
  int err = 0;
  double loss = 0.0;
  for (int j = 0, i = 0; i < l; i++) {
    double d = G[i] + y[i] * lambda_eq + 1.0;
    if (d < 0) err++;
    if (d < (1 - eps)) loss += (1 - d * y[i]);
    if (is_upper_bound (i)) out_model->bsv++;
    if (!is_lower_bound (i)) {
      out_model->add (alpha[i] * y[i], copy_feature_node (x[i]));
      out_model->svindex[j++] = i;
    }
  }

  // delete all allocated memory
  delete [] G;
  delete [] alpha_status;
  delete [] alpha;
  delete [] active2index;
  delete [] index2active;
  delete [] shrink_iter;
  delete gram_matrix;

  G = alpha = NULL;
  alpha_status = active2index = index2active = shrink_iter = NULL;
  active2index = NULL;
  gram_matrix = NULL;

  // finish all 
  fprintf (stdout, "\nDone! %d iterations\n\n", iter);
  fprintf (stdout, "Number of SVs (BSVs)\t\t%d (%d)\n", out_model->l,
	   out_model->bsv);
  fprintf (stdout, "Empirical Risk:\t\t\t%g (%d/%d)\n", 1.0 * err / l, err,
	   l);
  fprintf (stdout, "L1 Loss:\t\t\t%g\n", loss);
  fprintf (stdout, "CPU Time:\t\t\t%s\n", timer.getDiff ());

  return out_model;
}

// experimental code
int
SVM_Solver::writeSnapShot (const char *filename)
{
  FILE *fp = fopen (filename, "w");
  if (fp == NULL) return 0;

  for (int i = 0; i < l; i++)
    fprintf (fp, "%.20g %.20g %d\n", G[i], alpha[i], alpha_status[i]);

  fclose (fp);
  return 1;
}

int
SVM_Solver::readSnapShot (const char *filename)
{
  FILE *fp = fopen (filename, "r");
  if (fp == NULL) return 0;

  for (int i = 0; i < l; i++) {
    int num = fscanf (fp, "%lf %lf %d", &G[i], &alpha[i], (int *) &alpha_status[i]);
    if (num != 3) break;
  }

  fclose (fp);
  return 1;
}
