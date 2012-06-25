#include "common.h"
#include "misc.h"
#include "model.h"
#include "example.h"
#include "base_example.h"
#include "kernel.h"
#include "param.h"

// $Id: svm_learn.cc,v 1.15 2000/12/08 15:01:02 taku-ku Exp $;

int
main (int argc, char **argv)
{
  Param param;

  if (!param.set (argc, argv) || argc < 3) {
    fprintf (stdout, "%s\n", COPYRIGHT);
    fprintf (stdout, "Try `svm_learn --help' for more information.\n");
    exit (EXIT_FAILURE);
  }

  fprintf (stdout, "%s\n", COPYRIGHT);

  if (param.verbose) {
    if (param.kernel_type == LINEAR) {
      fprintf (stdout, "kernel_type:\t\tlinear\n");
    } else if (param.kernel_type == POLY) {
      fprintf (stdout, "kernel_type:\t\tpolynomial\n");
    } else {
      fprintf (stdout, "kernel_type:\t\tunknown\n");
    }
    fprintf (stdout, "cache_size:\t\t%g\n", param.cache_size);
    fprintf (stdout, "param_g:\t\t%g\n", param.param_g);
    fprintf (stdout, "param_s:\t\t%g\n", param.param_s);
    fprintf (stdout, "param_r:\t\t%g\n", param.param_r);
    fprintf (stdout, "degree:\t\t\t%d\n", param.degree);
    fprintf (stdout, "eps:\t\t\t%g\n", param.eps);
    fprintf (stdout, "C:\t\t\t%g\n", param.C);
    if (param.solver_type == SMO)
      fprintf (stdout, "solver_type:\t\tSMO+SVM_Light\n");
    else
      fprintf (stdout, "solver_type:\t\tunknown\n");
  }

  Example example;
  if (!example.read (argv[argc - 2])) {
    fprintf (stderr, "%s: %s: No such file or directory\n", argv[0],
	     argv[argc - 2]);
    exit (EXIT_FAILURE);
  }

  if (param.verbose)
    fprintf (stdout, "feature_type:\t\t%s\n\n",
	     example.feature_type == BINARY_FEATURE ? "binary" : "double");

  Model *model = example.learn (param);

  if (!model) {
    fprintf (stderr, "%s: Unexpected error occurs\n", argv[0]);
    exit (EXIT_FAILURE);
  }

  if (!model->write (argv[argc - 1])) {
    fprintf (stderr, "%s: %s: permission denied\n", argv[0], argv[argc - 1]);
    exit (EXIT_FAILURE);
  }

  if (param.verbose) {
    fprintf (stdout, "Margin:\t\t\t\t%g\n", model->estimateMargin ());
    double h = model->estimateVC ();
    fprintf (stdout, "Estimated VC dimension:\t\t%g\n", h);
    fprintf (stdout, "Estimated VC bound (n=0.05):\t%g\n",
	     sqrt ((h * (log (2 * example.size () / h) + 1) - log (0.05 / 4))
		   / example.size ()));
    fprintf (stdout, "Leave one out bound:\t\t%g (%d/%d)\n",
	     1.0 * model->getSVnum () / example.size (), model->getSVnum (),
	     example.size ());
  }

  delete model;

  return EXIT_SUCCESS;
}
