#include "example.h"
#include "misc.h"
#include "common.h"
#include "base_solver.h"
#include "svm_solver.h"
#include "svr_solver.h"

// $Id: example.cc,v 1.15 2001/01/16 20:13:06 taku-ku Exp $;

namespace TinySVM {

Model *
Example::learn (const Param & p)
{
  if (l == 0) return NULL;

  BaseSolver *solver;

  try {
    switch (p.solver_type) {
    case SVM:
      solver = new SVM_Solver (*this, p);
      break;
    case SVR:
      solver = new SVR_Solver (*this, p);
      break;
    default:
      fprintf (stderr, "Example::learn(): Unknown solver type [%d]\n", p.solver_type);
      return NULL;
    }
  }

  catch (...) {
    fprintf (stderr, "Example::learn(): Out of memory\n");
    exit (EXIT_FAILURE);
  }

  // learn
  Model *m = solver->learn ();
  delete solver;

  return m;
}

int
Example::read (const char *filename, const char *mode, const int offset)
{
  FILE *fp = fopen (filename, mode);
  if (!fp) return 0;

  fseek (fp, offset, SEEK_SET);
  char *buf;
  int line = 0;
  while ((buf = readLine (fp)) != NULL) {
    line++;
    if (!add (buf)) {
      fprintf (stderr, "Example::read(): Format error in file %s, line %d\n",
	       filename, line);
      fclose (fp);
      return 0;
    }
  }
  fclose (fp);

  return 1;
}

int
Example::write (const char *filename, const char *mode, const int offset)
{
  FILE *fp = fopen (filename, mode);
  if (!fp) return 0;

  for (int i = 0; i < l; i++) {
    fprintf (fp, "%.16g", y[i]);
    for (feature_node * node = x[i]; node->index != -1; node++)
      fprintf (fp, " %d:%.8g", node->index, node->value);
    fprintf (fp, "\n");
  }

  fclose (fp);
  return 1;
}

}
