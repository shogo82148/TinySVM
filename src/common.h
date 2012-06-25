#ifndef _COMMON_H
#define _COMMON_H

#define COPYRIGHT  "TinySVM - tiny SVM package\nCopyright (C) 2000-2001 Taku Kudoh All rights reserved.\n"

template <class T> inline T min(T x, T y) { return (x < y) ? x : y; }
template <class T> inline T max(T x, T y) { return (x > y) ? x : y; }
template <class T> inline void swap(T &x, T &y) { T z = x; x = y; y = z; }
template <class S, class T> inline void clone(T*& dst, S* src, int n)
{
  dst = new T [n];
  memcpy((void *)dst, (void *)src, sizeof(T)*n);
}

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CONFIG_BCC32_H
#include "config.h.bcc32"
#endif

#ifdef HAVE_CONFIG_MINGW32MSVC_H
#include "config.h.mingw32msvc"
#endif

#ifdef HAVE_CSTDIO
#include <cstdio>
#else
#ifdef STDC_HEADERS 
#include <stdio.h>
#endif
#endif

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#else
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#endif

#ifdef HAVE_CCTYPE
#include <cctype>
#else
#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif
#endif

#ifdef HAVE_CSTRING
#include <cstring>
#else
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#endif

#ifdef HAVE_CMATH
#include <cmath>
#else
#ifdef HAVE_MATH_H
#include <math.h>
#endif
#endif

#ifdef HAVE_CUNISTD
#include <cunistd>
#else
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_TIMES_H
#include <sys/times.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#ifndef HAVE_GETOPT
#include "getopt.h"
#endif

#ifndef HAVE_GETOPT_LONG
#include "getopt.h"
#endif

#ifndef HAVE_MEMSET
#ifdef HAVE_BZERO
#define memset(a,b,c) bzero((a),(c))
#else
template <class T> inline void memset(T* &a, int b, int c)
{
   for (int i = 0; i < c; i++) a[i] = b;
}
#endif
#endif

#ifndef HAVE_MEMCPY
#ifdef HAVE_BCOPY
#define memcpy(a,b,c) bcopy((b),(a),(c))
#else
template <class T> inline void memcpy(T* &a, T &b, int c)
{
   for (int i = 0; i < c; i++) a[i] = b[i];
}
#endif
#endif

#define EPS_A 1e-12
#ifndef HUGE_VAL
#define HUGE_VAL 1e+37
#endif

#define INF HUGE_VAL
#define MAXLEN 1024

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#endif
