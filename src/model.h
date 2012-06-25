#ifndef _MODEL_H
#define _MODEL_H
#include "base_example.h"
#include "param.h"

// $Id: model.h,v 1.11 2000/12/06 14:52:51 taku-ku Exp $;

class Model: public BaseExample
{
 private:
  double margin;
  double vc;

  void   *kernel;
  // It is very ad-hoc solution, this pointer is handled as *Classifer 
  // we must cast it into *Classifer using static_cast<> operation. 

 public:
  double b;
  int    bsv;
  int    *svindex;
  Param  param;

  int read         (const char *,  const char *mode = "r", int offset = 0);
  int write        (const char *, const char *mode = "w", int offset = 0);
  int writeSVindex (const char *,  const char *mode = "r", int offset = 0);
  int clear();
  
  double classify(const char *);
  double classify(const feature_node *);
  double estimateMargin();
  double estimateVC();

  const int *getSVindex() const { return (const int *)svindex; };
  int getSVnum() { return this->l; };

  Model();
  Model(const Param &);
  ~Model();

  Model& operator=(Model &m);
};
#endif
