#include "model.h"
#include "common.h"
#include "kernel.h"
#include "classifier.h"

// $Id: model.cc,v 1.17 2000/12/09 07:15:26 taku-ku Exp $;

Model::Model ()
{
  margin = vc = bsv = 0;
  kernel = NULL;
  feature_type = BINARY_FEATURE;
  svindex = NULL;
}

Model::Model (const Param & p)
{
  margin = vc = bsv = 0;
  kernel = NULL;
  feature_type = BINARY_FEATURE;
  svindex = NULL;
  param = p;
}

Model::~Model ()
{
  if (kernel)  delete static_cast <Classifier *>(kernel);
  if (svindex) delete [] svindex;
}

int
Model::clear ()
{
  if (kernel)  delete static_cast <Classifier *>(kernel);
  if (svindex) delete [] svindex;
  return BaseExample::clear ();
}

Model & Model::operator = (Model & m) 
{
  if (this != &m) {
    BaseExample::operator = (m);
    margin = vc = 0;
    feature_type = BINARY_FEATURE;
    param = m.param;
    if (kernel)  delete static_cast <Classifier *>(kernel);
    kernel = NULL;
  }

  return *this;
}

double
Model::classify (const char *s)
{
  if (!kernel)
    kernel = static_cast <void *>(new Classifier (*(dynamic_cast <BaseExample *>(this)), param));
  return static_cast <Classifier *>(kernel)->getDistance (s) - b;
}

double
Model::classify (const feature_node * f)
{
  if (!kernel)
    kernel = static_cast <void *>(new Classifier (*(dynamic_cast <BaseExample *>(this)), param));
  return static_cast <Classifier *>(kernel)->getDistance (f) - b;
}

double
Model::estimateMargin ()
{
  if (margin) return margin;
  if (!kernel)
    kernel = static_cast <void *>(new Classifier (*(dynamic_cast <BaseExample *>(this)), param));
  double m = 0;
  for (int i = 0; i < l; i++) {
    m += y[i] * static_cast < Classifier * >(kernel)->getDistance (x[i]);
  }

  margin = 1.0 / sqrt (m);
  return margin;
}

double
Model::estimateVC ()
{
  if (vc) return vc;
  if (!margin) margin = estimateMargin ();
  if (!kernel)
    kernel = static_cast <void *>(new Classifier (*(dynamic_cast <BaseExample *>(this)), param));
  feature_node *org = new feature_node[1];
  org[0].index = -1;
  org[0].value = 0;
  double orgsq = static_cast < Classifier * >(kernel)->getKernel (org, org);

  double maxd = -INF;
  for (int i = 0; i < l; i++) {
    maxd =
      max (maxd,
	   sqrt (static_cast <Classifier *>(kernel)->getKernel (x[i],x[i]) 
		 - 2 * static_cast <Classifier *>(kernel)->getKernel (x[i], org) + orgsq));
  }
  delete org;
  vc = (maxd * maxd) / (margin * margin) + 1;
  return vc;
}

int
Model::read (const char *filename, const char *mode, int offset)
{
  FILE *fp = fopen (filename, mode);
  if (!fp) return 0;

  fseek (fp, offset, SEEK_SET);
  char tbuf[1024];
  int tmpl;
  fgets (tbuf, 1024, fp);
  fscanf (fp, "%d%*[^\n]\n", &param.kernel_type);
  fscanf (fp, "%d%*[^\n]\n", &param.degree);
  fscanf (fp, "%lf%*[^\n]\n", &param.param_g);
  fscanf (fp, "%lf%*[^\n]\n", &param.param_s);
  fscanf (fp, "%lf%*[^\n]\n", &param.param_r);
  fscanf (fp, "%s%*[^\n]\n", tbuf);
  fscanf (fp, "%d%*[^\n]\n", &tmpl);
  tmpl--;
  fscanf (fp, "%lf%*[^\n]\n", &b);

  char *buf;
  int line = 0;
  while ((buf = readLine (fp)) != NULL && line < tmpl) {
    line++;
    if (!this->add (buf)) {
      fprintf (stderr, "WARNING: Format Error in file [%s] line [%d]\n",
	       filename, line);
      fclose (fp);
      return 0;
    }
  }
  fclose (fp);

  // make kernel
  kernel = static_cast <void *>(new Classifier (*(dynamic_cast <BaseExample *>(this)), param));

  return 1;
}

int
Model::write (const char *filename, const char *mode, int offset)
{
  FILE *fp = fopen (filename, mode);
  if (!fp) return 0;

  //   fprintf(fp,"SVM-light Version V3.02\n");
  fprintf (fp, "TinySVM Version %s\n", VERSION);
  fprintf (fp, "%d # kernel type\n", param.kernel_type);
  fprintf (fp, "%d # kernel parameter -d\n", param.degree);
  fprintf (fp, "%.8g # kernel parameter -g\n", param.param_g);
  fprintf (fp, "%.8g # kernel parameter -s\n", param.param_s);
  fprintf (fp, "%.8g # kernel parameter -r\n", param.param_r);
  fprintf (fp, "empty # kernel parameter -u\n");
  fprintf (fp, "%d # number of support vectors\n", l + 1);	// triky, svm_light must be l+1
  fprintf (fp, "%.16g # threshold b\n", b);

  for (int i = 0; i < l; i++) {
    fprintf (fp, "%.16g", y[i]);
    for (feature_node * node = x[i]; node->index != -1; node++)
      fprintf (fp, " %d:%.8g", node->index, node->value);
    fprintf (fp, "\n");
  }

  fclose (fp);
  return 1;
}

int
Model::writeSVindex (const char *filename, const char *mode, int offset)
{
  if (!svindex)
    return 0;
  FILE *fp = fopen (filename, mode);
  if (!fp)
    return 0;

  for (int i = 0; i < l; i++) {
    fprintf (fp, "%d %.16g\n", svindex[i], y[i]);
  }

  fclose (fp);
  return 1;
}
