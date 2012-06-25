#ifndef _COMMON_H
#define _COMMON_H

#define COPYRIGHT  "TinySVM - tiny SVM package\nCopyright (C) 2000 Taku Kudoh All rights reserved.\n"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CONFIG_BCC32_H
#include "config.h.bcc32"
#endif

#ifdef STDC_HEADERS
#include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_MATH_H
#include <math.h>
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
#endif
#endif

#define EPS_A 1e-12
#ifndef DBL_MAX
#define DBL_MAX 1e+37
#endif
#define INF DBL_MAX
#define MAXLEN 1024

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

template <class T> inline T min(T x,T y) { return (x < y) ? x : y; }
template <class T> inline T max(T x,T y) { return (x > y) ? x : y; }

#endif
