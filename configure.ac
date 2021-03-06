dnl Process this file with autoconf to produce a configure script.
AC_INIT(src/svm_learn.cpp)
AM_INIT_AUTOMAKE(TinySVM,0.09)

# Before making a release, the LTVERSION string should be modified.
# The string is of the form C:R:A.
# - If interfaces have been changed or added, but binary compatibility has
#   been preserved, change to C+1:0:A+1
# - If binary compatibility has been broken (eg removed or changed interfaces)
#   change to C+1:0:0
# - If the interface is the same as the previous version, change to C:R+1:A

LTVERSION="3:0:2"
AC_SUBST(LTVERSION)

AC_MSG_CHECKING([whether configure should try to set CFLAGS])
if test "x${CFLAGS+set}" = xset; then
   enable_cflags_setting=no
else
   enable_cflags_setting=yes
fi
AC_MSG_RESULT($enable_cflags_setting)
AC_MSG_CHECKING([whether configure should try to set CXXFLAGS])
if test "x${CXXFLAGS+set}" = xset; then
   enable_cxxflags_setting=no
else
   enable_cxxflags_setting=yes
fi
AC_MSG_RESULT($enable_cxxflags_setting)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_GCC_TRADITIONAL
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_C_INLINE

AC_CANONICAL_HOST
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
AC_ISC_POSIX
AC_LANG_CPLUSPLUS
AC_CONFIG_MACRO_DIR([m4])

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS(stdio.h string.h math.h stdlib.h unistd.h ctype.h \
		getopt.h sys/types.h sys/times.h windows.h libintl.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T

dnl Checks for libraries.
AC_CHECK_LIB(m,pow)
AC_CHECK_LIB(m,exp)

dnl Checks for library functions
AC_CHECK_FUNCS(isspace getopt getopt_long memset bzero memcpy bcopy pow sqrt)
AC_FUNC_MALLOC

dnl
dnl Check for GNU make
dnl
AC_MSG_CHECKING(whether make is GNU Make)
if $ac_make --version 2>/dev/null | grep '^GNU Make ' >/dev/null ; then
        AC_MSG_RESULT(yes)
else
        AC_MSG_RESULT(no)
        if test "$host_vendor" = "sun" ; then
                AC_MSG_ERROR("SUN make does not work for building TinySVM. Please install GNU make")
        fi
fi

AC_DEFUN(ADD_CC_FLAG, [
  AC_MSG_CHECKING(whether ${CC-cc} accepts $1)
  AC_LANG_SAVE
  AC_LANG_C
  XCFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS $1"
  AC_TRY_LINK([], [], 
	[AC_MSG_RESULT([ ok, adding $1 to CFLAGS])],
	[CFLAGS="$XCFLAGS"])
  AC_LANG_RESTORE
])

AC_DEFUN(ADD_CXX_FLAG, [
  AC_MSG_CHECKING(whether ${CXX-c++} accepts $1)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  XCXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $1"
  AC_TRY_LINK([], [], 
	[AC_MSG_RESULT([ ok, adding $1 to CXXFLAGS])],
	[CXXFLAGS="$XCXXFLAGS"])
  AC_LANG_RESTORE
])

dnl
dnl check gcc
dnl
if test "$GCC" = "yes"; then
   if test "$enable_cflags_setting" = "yes"; then
      CFLAGS="-Wall -O9 -funroll-all-loops -finline -ffast-math"
   fi
   if test "$enable_cxxflags_setting" = "yes"; then
      CXXFLAGS="-Wall -O9 -funroll-all-loops -finline -ffast-math"
   fi
fi

# On Intel systems with gcc, we may need to compile with -mieee-fp to
#  get full support for IEEE floating point.
#
# On Alpha/OSF systems, we need -mieee.
#
# On AIX systems, we need to limit the amount of stuff that goes in
# the TOC.
case "$host" in
changequote(,)dnl
  i[3456789]86-*-*)
changequote([,])dnl
    ADD_CC_FLAG(-mieee-fp)
    ADD_CXX_FLAG(-mieee-fp)
  ;;
  alpha*-*-*)
    ADD_CC_FLAG(-mieee)
    ADD_CXX_FLAG(-mieee)
  ;;
  *ibm-aix4*)
    ADD_CC_FLAG(-mminimal-toc)
    ADD_CXX_FLAG(-mminimal-toc)
  ;;
esac

#
# C++ compiler feature check start here
#

# check for const_cast
AC_MSG_CHECKING([if ${CXX-c++} supports template <class T> (required)])
AC_TRY_COMPILE(
[
   template <class T> T foo (T &i) { return i++; };
],[
   int i = 0;
   double d = 0.0;
   foo(i); foo(d);
],[
  ac_template=yes
],[
  AC_WARN(${CXX-c++} template <class T> does not work)
  config_error=yes
])
AC_MSG_RESULT([$ac_template])

# check for const_cast
AC_MSG_CHECKING([if ${CXX-c++} supports const_cast<> (required)])
AC_TRY_COMPILE(
[
   class foo;
],[
   const foo *c=0;
   foo *c1=const_cast<foo*>(c);
],[
  ac_const_cast=yes
],[
  AC_WARN(${CXX-c++} const_cast<> does not work)
  config_error=yes
])
AC_MSG_RESULT([$ac_const_cast])

# check for static_cast<>
AC_MSG_CHECKING(if ${CXX-c++} supports static_cast<> (required))
AC_TRY_COMPILE(
[
   class foo;
],[
   foo *c = 0;
   void *c1 = static_cast<void *>(c);
],[
  ac_static_cast=yes
],[
  AC_WARN(${CXX-c++} static_cast<> does not work)
  config_error=yes
])
AC_MSG_RESULT([$ac_static_cast])

# check for dynamic_cast<>
AC_MSG_CHECKING(if ${CXX-c++} supports dynamic_cast<> (required))
AC_TRY_COMPILE(
[
   class foo {};
   class bar: public foo {};
],[
   bar *c = 0;
   foo *c1 = dynamic_cast<foo *>(c);
],[
  ac_dynamic_cast=yes
],[
  AC_WARN(${CXX-c++} dynamic_cast<> does not work)
  config_error=yes
])
AC_MSG_RESULT([$ac_dynamic_cast])

# check for bool
AC_MSG_CHECKING(if ${CXX-c++} supports bool (required))
AC_TRY_COMPILE(
[
],[
   bool b=true;
   bool b1=false;
],[
  ac_bool=yes
],[
  config_error=yes
  AC_WARN(bool type is not supported by your compiler)
])
AC_MSG_RESULT([$ac_bool])

# check for namespaces
AC_MSG_CHECKING(if ${CXX-c++} supports namespaces (required) )
AC_TRY_COMPILE(
[
namespace Foo { struct A {}; }
using namespace Foo;
],[
A a;
],[
 ac_namespaces=yes
dnl AC_DEFINE(HAVE_CXX_NAMESPACE)
],[
 config_error=yes
 ac_namespaces=no
])
AC_MSG_RESULT([$ac_namespaces])

# namespace
if test "x$ac_namespaces" = xyes ; then
AC_MSG_CHECKING(if ${CXX-c++} uses std namespace )
AC_TRY_COMPILE(
[
#include <iostream>
namespace std{}
using namespace std;
],[
cout << "test" << endl;
],[
 ac_std_namespace=yes
 AC_DEFINE(HAVE_CXX_STD_NAMESPACE)
],[
 ac_std_namespace=maybe
])
if test "x$ac_std_namespace" = xmaybe ; then
AC_TRY_COMPILE(
[
#include <iostream.h>
namespace std{}
using namespace std;
],[
cout << "test" << endl;
],[
 ac_std_namespace=yes
 AC_DEFINE(HAVE_CXX_STD_NAMESPACE)
],[
 ac_std_namespace=no
])
fi
AC_MSG_RESULT([$ac_std_namespace])
fi

AC_MSG_CHECKING(if ${CXX-c++} environment provides all required features)
if test "x$config_error" = xyes ; then
  AC_MSG_RESULT([no])
  AC_MSG_ERROR([Your compiler is not powerful enough to compile TinySVM. \
	If it should be, see config.log for more information of why it failed.])
fi
AC_MSG_RESULT([yes])

#----------------------------------------------------------------
# Look for java (From SWIG)
#----------------------------------------------------------------
AC_ARG_WITH(java, [  --with-java=path   Set location of Java],[JAVABIN="$withval"], [JAVABIN=])
AC_ARG_WITH(javac, [  --with-javac=path Set location of Javac],[JAVACBIN="$withval"], [JAVACBIN=])
AC_ARG_WITH(jar, [  --with-jar=path     Set location of Jar],[JARBIN="$withval"], [JARBIN=])

if test -z "$JAVABIN" ; then
  AC_CHECK_PROGS(JAVA, java)
else
  JAVA="$JAVABIN"
fi

if test -z "$JAVACBIN" ; then
  AC_CHECK_PROGS(JAVAC, javac)
else
  JAVAC="$JAVACBIN"
fi

if test -z "$JARBIN" ; then
  AC_CHECK_PROGS(JAR, jar)
else
  JAR="$JAVRBIN"
fi

AC_MSG_CHECKING(for java include file jni.h)
AC_ARG_WITH(javaincl, [  --with-javaincl=path    Set location of Java include directory], [JAVAINCDIR="$withval"], [JAVAINCDIR=])

if test -z "$JAVAINCDIR"; then
  JAVAINCDIR="/usr/jdk*/include /usr/local/jdk*/include /opt/jdk*/include /usr/java/include /usr/local/java/include /opt/java/include /usr/include/java /usr/local/include/java /usr/include /usr/local/include /usr/lib/jvm/java-*/include"
fi

BUILD_OS=$(echo $(uname -s | tr "[A-Z]" "[a-z]"))

JAVAINC=""
for d in $JAVAINCDIR ; do
  if test -r $d/jni.h ; then
    AC_MSG_RESULT($d)
    JAVAINC="-I$d -I$d/$BUILD_OS"
    break
  fi
done

AC_SUBST(JAVA)
AC_SUBST(JAVAC)
AC_SUBST(JAVAINC)
#------------------------------------------------------

AM_CONFIG_HEADER(config.h)
AC_OUTPUT([Makefile
           src/Makefile 
	   src/Makefile.msvc
           tests/Makefile 	   
	   man/Makefile 
	   doc/Makefile 
	   java/Makefile
	   perl/Makefile.PL
	   ruby/extconf.rb
	   tinysvm.pc
	   tinysvm.spec])
