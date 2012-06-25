#include "gram_matrix.h"

// $Id: gram_matrix.cc,v 1.9 2000/12/08 15:01:02 taku-ku Exp $;

GramMatrix::GramMatrix (const BaseExample & prob, const Param & param):
Kernel (prob, param)
{
  cache_binary = NULL;
  cache_normal = NULL;
  binary_kernel_cache = NULL;
  buf = NULL;

  miss = hit = size = 0;

  // copy, 
  _x = new feature_node *[l];
  _y = new double[l];
  
  for (int i = 0; i < l; i++) {
    _x[i] = (feature_node *) x[i];
    _y[i] = y[i];
  }

  // making cache
  if (feature_type == BINARY_FEATURE) {
    binary_kernel_cache = new int[pack_d + 1];
    for (int i = 0; i < (pack_d + 1); i++)
      binary_kernel_cache[i] = (int) (this->getKernel ((double) i));

    if (pack_d <= 255) {
      cache_binary = new Cache <unsigned char>(l, 0.9 * param.cache_size);
      cache_normal = new Cache <double>(l, 0.1 * param.cache_size);
      size = cache_binary->size + cache_normal->size;
      _getQ = &GramMatrix::_getQ_binary_char;
    } else {
      cache_normal = new Cache <double>(l, param.cache_size);
      size = cache_normal->size;
      _getQ = &GramMatrix::_getQ_binary_double;
    }
  } else {
    cache_normal = new Cache <double>(l, param.cache_size);
    size = cache_normal->size;
    _getQ = &GramMatrix::_getQ_normal;
  }
}

GramMatrix::~GramMatrix ()
{
  if (binary_kernel_cache) delete [] binary_kernel_cache;
  if (buf)                 delete [] buf;
  if (cache_binary)        delete cache_binary;
  if (cache_normal)        delete cache_normal;
  delete [] _x;
  delete [] _y;
}

double *
GramMatrix::_getQ_normal (const int i, const int *active2index)
{
  double *data;

  if (cache_normal->getData (i, &data)) {
    hit++;
  } else {
    int j;
    for (int n = 0; (j = active2index[n]) != -1; n++)
      data[j] =
	(_y[i] * _y[j] * (this->*_getKernel) (dot_normal (_x[i], _x[j])));
    miss++;
  }

  return data;
}

double *
GramMatrix::_getQ_binary_char (const int i, const int *active2index)
{
  double *data1;

  if (cache_normal->getData (i, &data1)) {
    hit++;
  } else {
    int j;
    unsigned char *data2;
    if (cache_binary->getData (i, &data2)) {
      for (int n = 0; (j = active2index[n]) != -1; n++)
	data1[j] = _y[i] * _y[j] * binary_kernel_cache[data2[j]];
      hit++;
    } else {
      for (int n = 0; (j = active2index[n]) != -1; n++) {
	data2[j] = (unsigned char) dot_binary (_x[i], _x[j]);
	data1[j] = _y[i] * _y[j] * binary_kernel_cache[data2[j]];
      }
      miss++;
    }
  }

  return data1;
}

double *
GramMatrix::_getQ_binary_double (const int i, const int *active2index)
{
  double *data;

  if (cache_normal->getData (i, &data)) {
    hit++;
  } else {
    int j;
    for (int n = 0; (j = active2index[n]) != -1; n++)
      data[j] =
	(_y[i] * _y[j] * binary_kernel_cache[dot_binary (_x[i], _x[j])]);
  }

  return data;
}

void
GramMatrix::updateQ (const int _size, const int *active2index)
{
  feature_node **new_x = new feature_node *[_size];
  double *new_y = new double[_size];

  for (int k, n = 0; (k = active2index[n]) != -1; n++) {
    new_x[n] = _x[k];
    new_y[n] = _y[k];
  }

  delete [] _x;  _x = new_x;
  delete [] _y;  _y = new_y;

  size = 0;
  if (cache_normal) {
    cache_normal->update (_size, active2index);
    size += cache_normal->size;
  }

  if (cache_binary) {
    cache_binary->update (_size, active2index);
    size += cache_binary->size;
  }
}
