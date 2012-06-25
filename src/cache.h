#ifndef _CACHE_H
#define _CACHE_H
#include "common.h"

// $Id: cache.h,v 1.10 2000/12/04 16:40:12 taku-ku Exp $;

// Kernel Cache
template<class T = double> class Cache
{
private:
  int l;
  double cache_mem_size;

  struct head_t
  {
    head_t *prev, *next;    // a cicular list
    int index;
    T*  data;
  };

  head_t*  head;
  head_t*  lru_head;
  head_t** index_to_head;

  head_t*  new_head;
  head_t** new_index_to_head;
 
  inline void move_to_last(head_t *h)
  {
    if(lru_head == h) {
      lru_head = lru_head->next;
    } else {
      // delete from current location
      h->prev->next = h->next;
      h->next->prev = h->prev;

      // insert to last position
      h->next = lru_head;
      h->prev = lru_head->prev;
      h->prev->next = h;
      h->next->prev = h;
    }
  }

  inline void move_to_lru(head_t *h)
  {
    if (lru_head == h) {
      // do nothing, h is JUST deleted next search
    } else {
      // delete from current location
      h->prev->next = h->next;
      h->next->prev = h->prev;

      // insert to last position
      h->next = lru_head;
      h->prev = lru_head->prev;
      h->prev->next = h;
      h->next->prev = h;

      // rewrite lru_head, h is deleted next sarch
      lru_head = h;
    }
  }

public:
  int size;

  Cache(int,double);
  ~Cache();

  inline int deleteData(const int index)
  {
    head_t *h = index_to_head[index];
    if (h != NULL) {
      move_to_lru(h);
      return 1;
    } 
    return 0;
  }

  inline int getData(const int index, T **data)
  {
    head_t *h = index_to_head[index];
    if (h != NULL) {
      move_to_last(h);
      *data = h->data;
      return 1;
    } else {
      h = lru_head;
      lru_head = lru_head->next;
      if (h->index!=-1) index_to_head[h->index] = 0;
      h->index = index;
      index_to_head[index] = h;
      *data = h->data;
      return 0;
    }
  }

  void update(const int, const int *);
};

template <class T> Cache<T>::Cache(int _l, double _size) :l(_l), cache_mem_size(_size)
{
  try {
    size = max(2, (int)(1024 * 1024 * cache_mem_size/(sizeof(T) * l)));
    size = min(size, l);
    head = new head_t[size];

    // build circle lsit
    for(int i = 0; i < size; i++) {
      head[i].next  = &head[i+1];
      head[i].prev  = &head[i-1];
      head[i].index = -1;
      head[i].data  = new T[l];
    }

    head[0].prev      = &head[size-1];
    head[size-1].next = &head[0];
    lru_head          = &head[0];

    index_to_head = new head_t *[l];
    for(int i = 0; i < l; i++) index_to_head[i] = NULL;
  }

  catch (...) {
    fprintf(stderr,"Cache::Cache(): Out of memory\n");
    exit(1);
  }  
}

template <class T> Cache<T>::~Cache()
{
  for(int i = 0; i < size; i++)
    delete[] head[i].data;
  delete[] head;
  delete[] index_to_head;
}

template <class T> void Cache<T>::update(const int _l, const int *active2index)
{
  // make old -> new index table
  int *index2active = new int [l];
  for (int i = 0; i < l; i++) index2active[i] = -1;
  for (int k,n = 0; (k = active2index[n]) != -1; n++) index2active[k] = n;

  // update l & size
  l = _l;
  size = max(2, (int)(1024 * 1024 * cache_mem_size/(sizeof(T) * l)));
  size = min(size, l);

  new_head = new head_t[size];
  for(int i = 0; i < size; i++) {
    new_head[i].next  = &new_head[i+1];
    new_head[i].prev  = &new_head[i-1];
    new_head[i].index = -1;
    new_head[i].data  = NULL;
  }
  new_head[0].prev      = &new_head[size-1];
  new_head[size-1].next = &new_head[0];

  new_index_to_head = new head_t *[l];
  for (int i = 0; i < l; i++) new_index_to_head[i] = NULL;

  int j = 0;
  for (head_t *p = lru_head ; ; p = p->next) {
    if (p->index == -1 || index2active[p->index] == -1) {
      delete [] p->data;
    } else {
      new_head[j].data = new T [l];
      for (int k,n = 0; (k = active2index[n]) != -1; n++) new_head[j].data[n] = p->data[k];
      delete [] p->data; // delete old contents
      new_head[j].index = index2active[p->index];
      new_index_to_head[index2active[p->index]] = &new_head[j];
      j++;
    }

    if (p == lru_head->prev) break;
  }

  lru_head = &new_head[j];
  for (int i = j; i < size; i++) new_head[i].data = new T[l];

  delete [] head;
  delete [] index_to_head;
  delete [] index2active;

  head          = new_head;
  index_to_head = new_index_to_head;
}

#endif
