#include "param.h"
#include "misc.h"
#include "common.h"

// $Id: param.cc,v 1.17 2000/12/08 18:23:01 taku-ku Exp $;
// default param

#define PARAM_MAX_SIZE 512

#define TINYSVM_OPTION_INFO "\nUsage: %s [options] training-file model-file\n\
\n\
Kernel Parameter:\n\
  -t, --kernel-type=TYPE             select type of kernel function.\n\
                                     TYPE:  0 - linear (w * x)  (default)\n\
                                            1 - polynomial (s w*x + r)^d\n\
  -d, --kernel-degree=INT            set INT for parameter d in polynomial kernel. (default 1)\n\
  -r, --kernel-param-r=FLOAT         set FLOAT for parameter r in polynomial kernel. (default 1)\n\
  -s, --kernel-param-s=FLOAT         set FLOAT for parameter s in polynomial kernel. (default 1)\n\
\n\
Optimization Parameter:\n\
  -m, --cache-size=FLOAT             set FLOAT for cache memory size (MB). (default 40.0)\n\
  -c, --cost=FLOAT                   set FLOAT for cost C of constraints violation,\n\
                                     trade-off between training error and margin. (default 1.0)\n\
  -e, --termination-criterion=FLOAT  set FLOAT for tolerance of termination criterion.\n\
                                     (default 0.001)\n\
  -H, --shrinking-size=INT           set INT for number of iterations a variable needs to\n\
                                     be optimal before considered for shrinking. (default 100)\n\
  -p, --shrinking-threshold=FLOAT    set FLOAT for threshold value of recursive-shrinking process.\n\
                                     if #working/#current becomes less than FLOAT,\n\
                                     rebuilding kernel cache. (default 0.8)\n\
  -f, --do-final-check=INT           do final optimality check for variables removed\n\
                                     by shrinking. (default 1)\n\
\n\
Miscellaneous:\n\
  -o, --sv-index-file=FILE           write SV indices to FILE.\n\
  -V, --verbose                      set verbose mode.\n\
  -v, --version                      show the version of TinySVM and exit.\n\
  -h, --help                         show this help and exit.\n\
\n"

static const char *short_options = "m:t:d:s:r:c:e:o:f:p:H:vVh";

static struct option long_options[] = {
  {"cache-size",                     required_argument, NULL, 'm'},
  {"kernel-type",                    required_argument, NULL, 't'},
  {"kernel-degree",                  required_argument, NULL, 'd'},
  {"kernel-param-s",                 required_argument, NULL, 's'},
  {"kernel-param-r",                 required_argument, NULL, 'r'},
  {"cost",                           required_argument, NULL, 'c'},
  {"termination-criterion",          required_argument, NULL, 'e'},
  {"sv-index-file",                  required_argument, NULL, 'o'},
  {"do-final-check",                 required_argument, NULL, 'f'},
  {"recursive-shrinking-threshold",  required_argument, NULL, 'p'},
  {"shrinking-size",                 required_argument, NULL, 'H'},
  {"verbose",                        no_argument,       NULL, 'V'},
  {"version",                        no_argument,       NULL, 'v'},
  {"help",                           no_argument,       NULL, 'h'},
  {NULL, 0, NULL, 0}
};

Param::Param ()
{
  kernel_type = LINEAR;
  solver_type = SMO;
  degree = 1;
  param_g = 1;
  param_s = 1;
  param_r = 1;
  shrink_size = 100;
  shrink_th = 0.8;
  cache_size = 40;
  C = 1;
  final_check = 1;
  eps = 0.001;
  verbose = 0;
}

int
Param::set (int argc, char **argv)
{
  if (argc == 0)
    return 0;

  extern char *optarg;
  extern int optind;
  optind = 1;

  while (1) {
    int opt = getopt_long (argc, argv, short_options, long_options, NULL);
    if (opt == EOF) break;

    switch (opt) {
    case 'm':
      cache_size = atof (optarg);
      break;
    case 't':
      kernel_type = atoi (optarg);
      break;
    case 'd':
      degree = atoi (optarg);
      break;
    case 's':
      param_s = atof (optarg);
      break;
    case 'r':
      param_r = atof (optarg);
      break;
    case 'c':
      C = atof (optarg);
      break;
    case 'e':
      eps = atof (optarg);
      break;
    case 'f':
      final_check = atoi (optarg);
      break;
    case 'p':
      shrink_th = atof (optarg);
      break;
    case 'H':
      shrink_size = atoi (optarg);
      break;
    case 'V':
      verbose = 1;
      break;
    case 'v':
      fprintf (stdout, "%s of %s\n%s\n", argv[0], VERSION, COPYRIGHT);
      exit (EXIT_SUCCESS);
      break;
    case 'h':
      fprintf (stdout, "%s", COPYRIGHT);
      fprintf (stdout, TINYSVM_OPTION_INFO, argv[0]);
      exit (EXIT_SUCCESS);
    default:
      return 0;
    }
  }

  return 1;
}

int
Param::set (const char *s)
{
  try {
    if (!s) return 1;

    char *tmp = "TinySVM::Param::set";
    char *buf = new char[strlen (s)];
    strcpy (buf, s);
    int len = strlen (buf);
    int j = 1;
    int i = 0;
    int si = 0;
    char *argv[PARAM_MAX_SIZE];
    argv[0] = tmp;

    while (i < len) {
      while (i < len && isspace (buf[i])) {
	si++;i++;  // skip beginning spaces
      };	

      while (i < len && !isspace (buf[i])) i++;	// skip contents
      buf[i++] = '\0';
      if (buf[si])
	argv[j++] = &buf[si];
      si = i;
      if (j >= PARAM_MAX_SIZE) {
	fprintf (stderr, "Param::set(): Option is too long, ignored\n");
	break;
      }
    }

    int r = set (j, argv);
    delete buf;
    return r;
  }

  catch (...) {
    fprintf (stderr, "Param::set(): Out of memory\n");
    exit (EXIT_FAILURE);
  }
}
