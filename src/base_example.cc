#include "base_example.h"
#include "common.h"

// $Id: base_example.cc,v 1.11 2000/12/08 15:01:02 taku-ku Exp $;

// misc function
int
comp_feature_node (const void *x1, const void *x2)
{
  feature_node *p1 = (feature_node *) x1;
  feature_node *p2 = (feature_node *) x2;
  return (p1->index > p2->index);
}

feature_node *
copy_feature_node (const feature_node * f)
{
  int i;
  for (i = 0; f[i].index != -1; i++);
  try {
    feature_node *r = new feature_node[i + 1];
    for (i = 0; f[i].index != -1; i++) {
      r[i].index = f[i].index;
      r[i].value = f[i].value;
    }
    r[i].index = -1;
    return r;
  }

  catch (...) {
    fprintf (stderr, "copy_feature_node(): Out of memory\n");
    exit (EXIT_FAILURE);
  }
}

feature_node *
str2feature_node (const char *s)
{
  int elmnum = 0;
  int len = strlen (s);

  for (int i = 0; i < len; i++)
    if (s[i] == ':') elmnum++;

  try {
    feature_node *_x = new feature_node[elmnum + 1];

    int j = 0;
    for (int i = 0; j < elmnum && i < len;) {
      while (i < len && isspace (s[i]))	i++;
      _x[j].index = atoi (s + i);
      while (i + 1 < len && s[i] != ':') i++;
      _x[j].value = atof (s + i + 1);
      j++;
      while (i < len && !isspace (s[i])) i++;
    }

    // dumy index
    _x[j].index = -1;
    _x[j].value = 0;
    return _x;
  }

  catch (...) {
    fprintf (stderr, "str2feature_node(): Out of memory\n");
    exit (EXIT_FAILURE);
  }
}

feature_node *
fix_feature_node (feature_node * _x)
{
  register int i;
  register int cindex = -1;
  register int sorted = 1;

  // check sort
  for (i = 0; (_x + i)->index != -1;) {
    if ((_x + i)->value == 0) {	// check zero value
      int j = 0;
      while ((_x + i + j)->index != -1) {
	(_x + i + j)->value = (_x + i + j + 1)->value;
	(_x + i + j)->index = (_x + i + j + 1)->index;
	j++;
      }
      continue;
    }

    if (cindex >= (_x + i)->index) sorted = 0;
    cindex = (_x + i)->index;
    i++;
  }

  // sort
  if (!sorted)
    qsort ((void *) _x, i, sizeof (feature_node), comp_feature_node);

  return _x;
}

BaseExample::BaseExample ()
{
  l = alloc_l = d = pack_d = strl = 0;
  stre = NULL;
  x = NULL;
  y = NULL;
  refcount = 0;
  feature_type = BINARY_FEATURE;
}

BaseExample::~BaseExample ()
{
  if (refcount == 0 && x) {
    for (int i = 0; i < l; i++)
      delete[]x[i];
  }
  if (x)
    delete[]x;
  if (y)
    delete[]y;
  if (stre)
    delete[]stre;
}

// copy constructor
BaseExample & BaseExample::operator = (BaseExample & e) {
  if (this != &e) {
    clear ();
    for (int i = 0; i < e.l; i++)
      add (e.y[i], copy_feature_node (e.x[i]));
  }
  return *this;
}

int
BaseExample::clear ()
{
  if (x) {
    for (int i = 0; i < l; i++) delete [] x[i];
    delete [] x;
  }
  if (y) delete [] y;
  l = alloc_l = d = pack_d = 0;
  x = NULL;
  y = NULL;
  return 0;
}

char *
BaseExample::readLine (FILE * fp)
{
  long len;
  int c;
  char *tstr;

  try {
    if (stre == NULL) {
      strl = MAXLEN;
      stre = new char[strl];
    }

    len = 0;
    tstr = stre;
    while (1) {
      if (len >= strl) {
	strl += MAXLEN;
	tstr = new char[strl];
	for (int i = 0; i < len; i++) tstr[i] = stre[i];
	delete []stre;
	stre = tstr;
      }

      c = fgetc (fp);

      if (c == '\n' || c == '\r') {
	tstr[len] = '\0';
	break;
      }

      if (c == EOF && feof (fp)) {
	tstr[len] = '\0';
	if (feof (fp) && len == 0)
	  tstr = NULL;
	break;
      }
      tstr[len++] = c;
    }
  }

  catch (...) {
    fprintf (stderr, "BaseExample::readLine(): Out of memory\n");
    exit (EXIT_FAILURE);
  }

  return tstr;
}

int
BaseExample::add (const double _y, feature_node * _x)
{
  int fnum = 0;
  try {
    feature_node *node = fix_feature_node ((feature_node *) _x);

    for (int i = 0; (node + i)->index != -1; i++) {
      if ((node + i)->value != 1)
	feature_type = DOUBLE_FEATURE;	// check feature type
      d = max (d, (node + i)->index);	// save max dimension
      fnum++;
    }

    pack_d = max (fnum, pack_d);

    if (!fnum) return 0; // empty node

    // malloc && realoc
    if (l == alloc_l) {
      alloc_l += MAXLEN;

      feature_node **tmp_x = new feature_node *[alloc_l];
      double *tmp_y = new double[alloc_l];

      for (int i = 0; i < l; i++) {
	tmp_x[i] = x[i];
	tmp_y[i] = y[i];
      }

      if (x)
	delete[]x;
      if (y)
	delete[]y;

      x = tmp_x;
      y = tmp_y;
    }

    y[l] = _y;
    x[l] = node;
    l++;

    return 1;
  }

  catch (...) {
    fprintf (stderr, "BaseExample::add(): Out of memory\n");
    exit (EXIT_FAILURE);
  }
}

int
BaseExample::add (const double _y, const char *s)
{
  return add (_y, (feature_node *) str2feature_node (s));
}

int
BaseExample::add (const char *s)
{
  double _y = 0;
  int len = strlen (s);

  int i;
  for (i = 0; i < len;) {
    while (isspace (s[i])) i++;
    _y = atof (s + i);
    while (i < len && !isspace (s[i])) i++;
    while (i < len && isspace (s[i])) i++;
    break;
  }

  return this->add (_y, (const char *) (s + i));
}
