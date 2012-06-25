#ifndef _BASE_EXAMPLE_H
#define _BASE_EXAMPLE_H
#include "misc.h"
#include <stdio.h>

// $Id: base_example.h,v 1.10 2000/12/06 14:52:51 taku-ku Exp $;
extern feature_node *str2feature_node   (const char *);
extern feature_node *fix_feature_node   (feature_node *);
extern feature_node *copy_feature_node  (const feature_node *);

class BaseExample
{
private:
  int   refcount;
  char *stre;
  int   strl;
  int   alloc_l;

public:
  int          l;	       // number of SVs
  int          d;              // dimension of SVs
  int          pack_d;         // packed dimension of SVs
  double       *y;	       // class label
  feature_node **x;	       // training data
  int          feature_type;   // feature type (bin or double)

  int ref() { return refcount++; };

  int add (const double, feature_node *);
  int add (const double, const char *);
  int add (const char *);
  int clear();
  int size() { return l; };

  virtual int read   (const char *,  const char *mode = "r", int offset = 0) = 0;
  virtual int write  (const char *, const char *mode = "w", int offset = 0) = 0;

  char *readLine(FILE *);

  // copy constructor
  BaseExample& operator=(BaseExample &);

  BaseExample();
  virtual ~BaseExample();
};
#endif
