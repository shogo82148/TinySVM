#ifndef _MODEL_H
#define _MODEL_H
#include "base_example.h"
#include "param.h"
#include "kernel.h"
#include "classifier.h"

// $Id: model.h,v 1.22 2001/08/24 13:07:52 taku-ku Exp $;

namespace TinySVM {

class Model: public BaseExample
{
 private:
  double margin;
  double vc;
  Param  param;
  Classifier *kernel;

 public:
  double b;
  double sphere;
  double loss;
  int    bsv;
   
  int read         (const char *,  const char *mode = "r", const int offset = 0);
  int write        (const char *,  const char *mode = "w", const int offset = 0);
  int clear();

  // classify
  double classify (const char *);
  double classify (const feature_node *);

  // model information
  double estimateMargin ();
  double estimateSphere ();
  double estimateVC ();
  double estimateXA (const double rho = 2.0);

  int compress (); // compress liner model

  int    getSVnum ()            { return this->l; };
  int    getBSVnum ()           { return this->bsv; };
  int    getTrainingDataSize () { return this->svindex_size; };
  double getLoss ()             { return this->loss; };

  Model();
  Model(const Param &);
  ~Model();

  Model& operator=(Model &m);
};

};
#endif

