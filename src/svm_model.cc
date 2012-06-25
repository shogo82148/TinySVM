#include "common.h"
#include "misc.h"
#include "model.h"
#include "example.h"
#include "base_example.h"
#include "kernel.h"
#include "param.h"

// $Id: svm_model.cc,v 1.2 2000/12/08 15:01:02 taku-ku Exp $;

const char *help_message = "\nUsage: svm_model [options] model-file\n\n\
options:\n\
  -n, --sv-num       show number of Support Vectors.\n\
  -m, --margin       show estimated margin.\n\
  -d, --vc-dimension show estimated VC dimension.\n\
  -v, --version      show the version of TinySVM and exit.\n\
  -h, --help         show this help and exit.\n ";

static const char *short_options = "anmdvh";

static struct option long_options[] = {
  {"sv-num",       no_argument, NULL, 'n'},
  {"margin",       no_argument, NULL, 'm'},
  {"vc-dimension", no_argument, NULL, 'd'},
  {"version",      no_argument, NULL, 'v'},
  {"help",         no_argument, NULL, 'h'},
  {NULL, 0, NULL, 0}
};

int
main (int argc, char **argv)
{
  int all = 1;
  int margin = 0;
  int vcdim = 0;
  int svnum = 0;

  while (1) {
    int opt = getopt_long (argc, argv, short_options, long_options, NULL);
    if (opt == EOF)
      break;

    switch (opt) {
    case 'n':
      all = 0;
      svnum = 1;
      break;
    case 'm':
      all = 0;
      margin = 1;
      break;
    case 'd':
      all = 0;
      vcdim = 1;
      break;
    case 'v':
      fprintf (stdout, "%s of %s\n%s\n", argv[0], VERSION, COPYRIGHT);
      exit (EXIT_SUCCESS);
    case 'h':
      fprintf (stdout, "%s%s", COPYRIGHT, help_message);
      exit (EXIT_SUCCESS);
    default:
      fprintf (stdout, "%s\n", COPYRIGHT);
      fprintf (stdout, "Try `svm_model --help' for more information.\n");
      exit (EXIT_SUCCESS);
    }
  }

  if (argc < 2) {
    fprintf (stdout, "%s\n", COPYRIGHT);
    fprintf (stdout, "Try `svm_model --help' for more information.\n");
    exit (EXIT_FAILURE);
  }

  Model model;
  model.read (argv[argc - 1]);
  fprintf (stdout, "File Name:\t\t\t%s\n", argv[argc - 1]);

  if (all || margin)
    fprintf (stdout, "Margin:\t\t\t\t%g\n", model.estimateMargin ());
  if (all || svnum)
    fprintf (stdout, "Number of SVs:\t\t\t%d\n", model.size ());
  if (all || vcdim)
    fprintf (stdout, "Estimated VC dimension:\t\t%g\n", model.estimateVC ());

  return EXIT_SUCCESS;
}
