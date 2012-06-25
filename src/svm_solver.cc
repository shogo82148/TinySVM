#include "svm_solver.h"
#include "common.h"
#include "example.h"
#include "classifier.h"
#include "timer.h"
#include "qp_solver.h"

// $Id: svm_solver.cc,v 1.27 2001/01/16 20:13:06 taku-ku Exp $;

namespace TinySVM {

Model *
SVM_Solver::learn ()
{
  if (example.class_type != BINARY_FEATURE) {
    fprintf (stderr, "SVM_Solver::learn(): This data have real-value class label.\n");    
    fprintf (stderr, "SVM_Solver::learn(): Use C-SVR mode for regression estimation.\n");
    return NULL;
  }

  try {
    Timer timer;
    double obj, rho;
    const double *y        = (const double *)example.y;
    const feature_node **x = (const feature_node **)example.x;
    double *alpha = new double [l];
    double *G     = new double [l];
    double *b     = new double [l];

    for (int i = 0; i < l; i++) {
      G[i] = b[i] = -1;
      alpha[i] = 0;
    }

    QP_Solver qp_solver;
    qp_solver.solve(example, param, b, alpha, G, rho, obj);

    // make output model
    Model *out_model = new Model (param);
    out_model->b = -rho;
    out_model->svindex = new int[l];

    int err = 0;
    double loss = 0.0;
    int bsv = 0;
    for (int j = 0, i = 0; i < l; i++) {
      double d = G[i] + y[i] * rho + 1.0;
      if (d < 0) err++;
      if (d < (1 - param.eps)) loss += (1 - d);
      if (alpha[i] >= param.C - EPS_A) bsv++; // upper bound
      if (alpha[i] > EPS_A) {  // free 
	out_model->add (alpha[i] * y[i], copy_feature_node (x[i]));
	out_model->svindex[j++] = i;
      }
    }

    out_model->bsv =  bsv;
    out_model->loss = loss;
    out_model->training_data_size = example.l;

    delete [] alpha;
    delete [] G;
    delete [] b;

    fprintf (stdout, "Number of SVs (BSVs)\t\t%d (%d)\n", out_model->l, out_model->bsv);
    fprintf (stdout, "Empirical Risk:\t\t\t%g (%d/%d)\n", 1.0 * err/l, err,l);
    fprintf (stdout, "L1 Loss:\t\t\t%g\n", loss);
    fprintf (stdout, "CPU Time:\t\t\t%s\n", timer.getDiff ());

    return out_model;
  }

  catch (...) {
    fprintf (stderr, "SVM_Solver::learn(): Out of memory\n");
    exit (EXIT_FAILURE);
  }
}

}
