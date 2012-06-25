#include "common.h"
#include "misc.h"
#include "model.h"
#include "example.h"
#include "base_example.h"
#include "kernel.h"
#include "param.h"

const char *help_message =
"\nUsage: svm_classify [options] test-file model-file\n\n\
options:\n\
  -t, --test-type=TYPE   set format of test file.\n\
                         0 -- with answser (default)\n\
                         1 -- without answser (interacitve mode)\n\
  -V, --verbose          set verbose mode.\n\
  -v, --version          show the version of TinySVM and exit.\n\
  -h, --help             show this help and exit.\n "; 

static const char *short_options = "t:Vvh";

static struct option long_options[] = {
  {"test-type", required_argument, NULL, 't'},
  {"verbose",   no_argument,       NULL, 'V'},
  {"version",   no_argument,       NULL, 'v'},
  {"help",      no_argument,       NULL, 'h'},
  {NULL, 0, NULL, 0}
};

int
main (int argc, char **argv)
{
  FILE *fp;
  int verbose = 0;
  int test_type = 0;

  extern char *optarg;

  while (1) {
    int opt = getopt_long (argc, argv, short_options, long_options, NULL);
    if (opt == EOF) break;

    switch (opt) {
    case 't':
      test_type = atoi (optarg);
      break;
    case 'V':
      verbose = 1;
      break;
    case 'v':
      fprintf (stdout, "%s of %s\n%s\n", argv[0], VERSION, COPYRIGHT);
      exit (EXIT_SUCCESS);
    case 'h':
      fprintf (stdout, "%s%s", COPYRIGHT, help_message);
      exit (EXIT_SUCCESS);
    default:
      fprintf (stdout, "%s\n", COPYRIGHT);
      fprintf (stdout, "Try `svm_classify --help' for more information.\n");
      exit (EXIT_SUCCESS);
    }
  }

  if (argc < 3) {
    fprintf (stdout, "%s\n", COPYRIGHT);
    fprintf (stdout, "Try `svm_classify --help' for more information.\n");
    exit (EXIT_FAILURE);
  }

  Model model;
  if (!model.read (argv[argc - 1])) {
    fprintf (stderr, "%s: %s: No such file or directory\n", argv[0], argv[argc - 1]);
    exit (EXIT_FAILURE);
  }

  if (!strcmp (argv[argc - 2], "-")) {
    fp = stdin;
  } else {
    if ((fp = fopen (argv[argc - 2], "r")) == NULL) {
      fprintf (stderr, "%s: %s: No such file or directory\n", argv[0], argv[argc - 2]);
      exit (EXIT_FAILURE);
    }
  }

  char *line;
  if (test_type == 0) {
    int all = 0;
    int ok = 0;
    while ((line = model.readLine (fp)) != NULL) {
      double y = 0;
      int len = strlen (line);
      int i = 0;
      while (isspace (line[i]))	i++;
      y = atof (line + i);
      while (i < len && !isspace (line[i])) i++;
      while (i < len && isspace (line[i])) i++;

      double r = model.classify ((const char *) (line + i));
      if (verbose) printf ("%g %g\n", y, r);
      fflush (stdout);
      if (y * r >= 0) ok++;
      all++;
    }
    printf ("Accuracy = %10.10f%% (%d/%d)\n", 100.0 * ok / all, ok, all);
  } else {
    while ((line = model.readLine (fp)) != NULL) {
      printf ("%g\n", model.classify ((const char *) (line)));
      fflush (stdout);
    }
  }

  fclose (fp);

  return EXIT_SUCCESS;
}
