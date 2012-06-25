#include "example.h"
#include "misc.h"
#include "common.h"
#include "base_solver.h"
#include "svm_solver.h"
#include "svr_solver.h"

// $Id: example.cc,v 1.21 2001/08/24 13:07:52 taku-ku Exp $;

namespace TinySVM {

Model *
Example::learn (const Param & p)
{
  if (l == 0) return 0;

  BaseSolver *solver;

  try {
    if (p.model[0]) {
      char *tmp = new char [strlen(p.model) + 5];
      strcpy (tmp, p.model);
      strcat (tmp, ".idx");
      fflush(stdout);

      if (readSVindex(tmp)) {
	TinySVM::Model *m = new TinySVM::Model;
	if (m->read (p.model)) rebuildSVindex (m);
	else fprintf (stderr, "Example::learn() cannot open %s, ignored.\n", p.model);
	delete m;
      } else {
	fprintf (stderr, "Example::learn() cannot open %s, ignored.\n", tmp);
      }

      delete [] tmp;
    }

    switch (p.solver_type) {
    case SVM:
      solver = new SVM_Solver (*this, p);
      break;
    case SVR:
      solver = new SVR_Solver (*this, p);
      break;
    default:
      fprintf (stderr, "Example::learn(): Unknown solver type [%d]\n", p.solver_type);
      return 0;
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
  svindex_size = l;

  return 1;
}

int
Example::write (const char *filename, const char *mode, const int offset)
{
  FILE *fp = fopen (filename, mode);
  if (!fp) return 0;

  for (int i = 0; i < l; i++) {
    fprintf (fp, "%.16g", y[i]);
    for (feature_node * node = x[i]; node->index >= 0; node++)
      fprintf (fp, " %d:%.16g", node->index, node->value);
    fprintf (fp, "\n");
  }

  fclose (fp);
  return 1;
}

int 
Example::rebuildSVindex (Model *m)
{
  if (!m ) return 0;

  alpha = resize (alpha, svindex_size, l, 0.0);
  G     = resize (G,     svindex_size, l, 0.0);

  for (int i = svindex_size; i < l; i++) {
    G[i] = y[i] * (m->classify(x[i]) + m->b) - 1;
    alpha[i] = 0;
  }

  svindex_size = l;
  return l;
}
}
