#include "param.h"
#include "misc.h"
#include "common.h"

// $Id: param.cc,v 1.30 2001/08/25 13:41:53 taku-ku Exp $;
// default param

#define PARAM_MAX_SIZE 512

#define TINYSVM_OPTION_INFO "\nUsage: %s [options] training-file model-file\n\
\n\
Solver Type:\n\
  -l, --solver-type=INT              select type of solver.\n\
                                     TYPE:  0 - C-SVM (default)\n\
                                            1 - C-SVR\n\
Kernel Parameter:\n\
  -t, --kernel-type=INT              select type of kernel function.\n\
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
  -H, --shrinking-size=INT           set INT for number of iterations variable needs to\n\
                                     be optimal before considered for shrinking. (default 100)\n\
  -p, --shrinking-eps=FLOAT          set FLOAT for initial threshold value of shrinking process.\n\
                                     (default 2.0)\n\
  -f, --do-final-check=INT           do final optimality check for variables removed\n\
                                     by shrinking. (default 1)\n\
  -i, --insensitive-loss=FLOAT       set FLOAT for epsilon in epsilon-insensitive loss function\n\
                                     used in C-SVR cost evaluation. (default 0.1)\n\
\n\
Miscellaneous:\n\
  -M, --model=FILE                   set FILE, FILE.idx for initial condition model file.\n\
  -I, --sv-index                     write SV index to MODEL.idx.\n\
  -W  --compress                     calculate vector w (w * x + b) directory instead of alpha.\n\
  -V, --verbose                      set verbose mode.\n\
  -v, --version                      show the version of TinySVM and exit.\n\
  -h, --help                         show this help and exit.\n\
\n"

static const char *short_options = "l:t:d:s:r:m:M:c:e:H:p:f:i:WIVvh";

static struct option long_options[] = {
  {"solver-type",             required_argument, NULL, 'l'},
  {"kernel-type",             required_argument, NULL, 't'},  
  {"kernel-degree",           required_argument, NULL, 'd'},
  {"kernel-param-s",          required_argument, NULL, 's'},
  {"kernel-param-r",          required_argument, NULL, 'r'},
  {"cache-size",              required_argument, NULL, 'm'},
  {"model",                   required_argument, NULL, 'M'},
  {"cost",                    required_argument, NULL, 'c'},
  {"termination-criterion",   required_argument, NULL, 'e'},
  {"shrinking-size",          required_argument, NULL, 'H'},
  {"shrinking-eps",           required_argument, NULL, 'p'},
  {"do-final-check",          required_argument, NULL, 'f'},
  {"insensitive-loss",        required_argument, NULL, 'i'},
  {"sv-index",                no_argument,       NULL, 'I'},
  {"compress",                no_argument,       NULL, 'W'},
  {"verbose",                 no_argument,       NULL, 'V'},
  {"version",                 no_argument,       NULL, 'v'},
  {"help",                    no_argument,       NULL, 'h'},
  {NULL, 0, NULL, 0}
};

extern char *optarg;
extern int optind;

namespace TinySVM {

Param::Param ()
{
  kernel_type = LINEAR;
  solver_type = SVM;
  degree = 1;
  param_g = 1;
  param_s = 1;
  param_r = 1;
  shrink_size = 100;
  shrink_eps = 2.0;
  cache_size = 40;
  insensitive_loss = 0.1;
  svindex = 0;
  C = 1;
  final_check = 1;
  eps = 0.001;
  verbose = 0;
  compress = 0;
  model[0] = 0;
}

Param::~Param() {};

int
Param::set (int argc, char **argv)
{
  if (argc == 0) return 0;

  optind = 1;

  while (1) {
    int opt = getopt_long (argc, argv, short_options, long_options, NULL);
    if (opt == EOF) break;

    switch (opt) {
    case 'l':
      solver_type = atoi(optarg);
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
    case 'm':
      cache_size = atof (optarg);
      break;
    case 'M':
      strcpy (model, optarg);
      break;
    case 'c':
      C = atof (optarg);
      break;
    case 'e':
      eps = atof (optarg);
      break;
    case 'H':
      shrink_size = atoi (optarg);
      break;
    case 'p':
      shrink_eps = atof (optarg);
      break;
    case 'f':
      final_check = atoi (optarg);
      break;
    case 'i':
      insensitive_loss = atof (optarg);
      break;
    case 'I':
      svindex = 1;
      break;
    case 'W':
     compress = 1;
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
    delete [] buf;
    return r;
  }

  catch (...) {
    fprintf (stderr, "Param::set(): Out of memory\n");
    exit (EXIT_FAILURE);
  }
}
};
