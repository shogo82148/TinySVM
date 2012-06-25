#ifndef _MISC_H
#define _MISC_H

enum { LINEAR, POLY };
enum { DOUBLE_FEATURE, BINARY_FEATURE };
enum { SMO, KA };

struct feature_node
{
  int index;
  double value;
};

#endif
