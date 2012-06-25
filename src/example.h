#ifndef _EXAMPLE_H
#define _EXAMPLE_H
#include "param.h"
#include "model.h"
#include "base_example.h"

// $Id: example.h,v 1.8 2000/12/06 14:52:51 taku-ku Exp $;

class Example: public BaseExample
{
public:
  int read  (const char *,  const char *mode = "r", int offset = 0);
  int write (const char *, const char *mode = "w", int offset = 0);

  Model *learn(const Param &);
  Model *learn(const Param *p) { return this->learn(*p); };
  Model *learn(const char *s) 
  {
    Param p;
    if (p.set(s)) return this->learn(p);
    else return NULL;
  }    
};
#endif
