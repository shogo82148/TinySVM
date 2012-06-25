#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "tinysvm.h"

#define TinySVM__Model TinySVM::Model
#define TinySVM__Example TinySVM::Example

/* $Id: TinySVM.xs,v 1.17 2001/08/24 13:07:49 taku-ku Exp $; */

MODULE = TinySVM		PACKAGE = Example

TinySVM::Example *
TinySVM::Example::new(...)
	PREINIT:
   	TinySVM::Example *e;
	CODE:
	 e = new TinySVM::Example();
	 if (items > 1) {
	   char *f = (char *) SvPV(ST(1), PL_na);
	   e->read(f);
	  }
	RETVAL = e;
	OUTPUT:
	RETVAL

int
TinySVM::Example::read(f, ...)
	char *f;
	PREINIT:
	int offset = 0;
	CODE:
	  if (items > 2) offset = (int) SvIV(ST(2));
	RETVAL = THIS->read(f,"r",offset);
	OUTPUT:
	RETVAL

int
TinySVM::Example::write(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"w");
	OUTPUT:
	RETVAL

int
TinySVM::Example::append(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"a");
	OUTPUT:
	RETVAL

TinySVM::Model*
TinySVM::Example::learn(opt)
	char *opt
	PREINIT:
	TinySVM::Param p;
	 char *CLASS = "Model";
	CODE:
	  if (items > 1) p.set(opt);
	  RETVAL = THIS->learn(p);
	OUTPUT:
	RETVAL

int
TinySVM::Example::add(...)
	PREINIT:
	double y;
	char *x;
	CODE:
	  if (items == 2) {
	      x = (char *)SvPV(ST(1),PL_na);
	      RETVAL = THIS->add(x);
	  } else if (items == 3) {
              y = (int)SvIV(ST(1));
	      x = (char *)SvPV(ST(2),PL_na);
	      RETVAL = THIS->add(y,x);	
	  } else {
	      RETVAL = 0;
    	  }
	OUTPUT:
	RETVAL

int
TinySVM::Example::remove(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->size()) 
	   RETVAL = THIS->remove(i);
         else {
	   warn( "Example::remove() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

double
TinySVM::Example::getY(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->size()) 
	   RETVAL = THIS->y[i];
         else  {
	   warn( "Example::getY() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

void
TinySVM::Example::getX(i)
        int i
	PREINIT:
	TinySVM::feature_node *f;
	char tmp[256];
	PPCODE:
	 if (i >= 0 && i < THIS->size()) {
	   f =  THIS->x[i];
	   for (int j = 0; f[j].index >= 0; j++) {
             sprintf(tmp,"%d:%g",f[j].index,f[j].value);
	     PUSHs(sv_2mortal(newSVpv(tmp,strlen(tmp))));
	   }
	  } else {
	   warn( "Example::getX() -- index is out of range" );
           XSRETURN_UNDEF;
          }
	  
double
TinySVM::Example::getAlpha(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->svindex_size)
	   RETVAL = THIS->alpha[i];
         else  {
	   warn( "Example::getAlpha() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

double
TinySVM::Example::getG(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->svindex_size)
	   RETVAL = THIS->G[i];
         else  {
	   warn( "Example::getG() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

int
TinySVM::Example::size()
	CODE:
	 RETVAL = THIS->size();
	OUTPUT:
	 RETVAL

int
TinySVM::Example::clear()
	CODE:
	 RETVAL = THIS->clear();
	OUTPUT:
	 RETVAL
	 
void
TinySVM::Example::DESTROY()


MODULE = TinySVM		PACKAGE = Model

TinySVM::Model *
TinySVM::Model::new(...)
	PREINIT:
   	TinySVM::Model *m;
	CODE:
	m = new TinySVM::Model();
	if (items > 1) {
	   char *f = (char *) SvPV(ST(1), PL_na);
	   m->read(f);
	}
	RETVAL = m;
	OUTPUT:
	RETVAL

int
TinySVM::Model::read(f, ...)
	char *f
	PREINIT:
	int offset = 0;
	CODE:
	if (items > 2) offset = (int) SvIV(ST(2));
	RETVAL = THIS->read(f,"r",offset);
	OUTPUT:
	RETVAL

int
TinySVM::Model::write(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"w");
	OUTPUT:
	RETVAL

int
TinySVM::Model::append(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"a");
	OUTPUT:
	RETVAL

int
TinySVM::Model::writeSVindex(f)
	char *f;
	CODE:
	RETVAL = THIS->writeSVindex(f,"w");
	OUTPUT:
	RETVAL

int
TinySVM::Model::appendSVindex(f)
	char *f;
	CODE:
	RETVAL = THIS->writeSVindex(f,"a");
	OUTPUT:
	RETVAL

int
TinySVM::Model::remove(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->size())
	   RETVAL = THIS->remove(i);
         else  {
	   warn( "Model::remove() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

double
TinySVM::Model::getY(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->size()) 
	   RETVAL = THIS->y[i];
         else  {
	   warn( "Model::getY() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

double
TinySVM::Model::getAlpha(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->getTrainingDataSize())
	   RETVAL = THIS->alpha[i];
         else  {
	   warn( "Model::getAlpha() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

double
TinySVM::Model::getG(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->getTrainingDataSize())
	   RETVAL = THIS->G[i];
         else  {
	   warn( "Model::getG() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

void
TinySVM::Model::getX(i)
        int i
	PREINIT:
	TinySVM::feature_node *f;
	char tmp[256];
	int j;
	PPCODE:
	 if (i >= 0 && i < THIS->size()) {
	   f =  THIS->x[i];
	   for (j = 0; f[j].index >= 0; j++) {
             sprintf(tmp,"%d:%g",f[j].index,f[j].value);
	     PUSHs(sv_2mortal(newSVpv(tmp,strlen(tmp))));
	   }
	  } else {
	   warn( "Example::getX() -- index is out of range" );
           XSRETURN_UNDEF;
          }

double
TinySVM::Model::classify(x)
        char *x
        CODE:
        RETVAL = THIS->classify(x);
        OUTPUT:
        RETVAL

double
TinySVM::Model::classify_list(...)
	PREINIT:
	int i;
	CODE:
	 struct TinySVM::feature_node *node  = 
	   (struct TinySVM::feature_node *)malloc(sizeof(struct TinySVM::feature_node) * (items + 1));
	 for (i = 0; i < items; i++) {
	   node[i].index = SvIV(ST(i+1));
	   node[i].value = 1;
	 }
	 node[i].index = -1;
	 RETVAL = THIS->classify(node);
         free(node);
	OUTPUT:
         RETVAL

int
TinySVM::Model::clear()
	CODE:
	 RETVAL = THIS->clear();
	OUTPUT:
	 RETVAL
	 
int
TinySVM::Model::compress()
	CODE:
	 RETVAL = THIS->compress();
	OUTPUT:
	 RETVAL

double
TinySVM::Model::estimateMargin()
	CODE:
	 RETVAL = THIS->estimateMargin();
	OUTPUT:
	 RETVAL
	
double
TinySVM::Model::estimateVC()
	CODE:
	 RETVAL = THIS->estimateVC();
	OUTPUT:
	 RETVAL

double
TinySVM::Model::estimateSphere()
	CODE:
	 RETVAL = THIS->estimateSphere();
	OUTPUT:
	 RETVAL
	 
int
TinySVM::Model::getBSVnum()
	CODE:
	 RETVAL = THIS->getBSVnum();
	OUTPUT:
	 RETVAL

int
TinySVM::Model::getTrainingDataSize()
	CODE:
	 RETVAL = THIS->getTrainingDataSize();
	OUTPUT:
	 RETVAL

double
TinySVM::Model::getLoss()
	CODE:
	 RETVAL = THIS->getLoss();
	OUTPUT:
	 RETVAL

double
TinySVM::Model::estimateXA(...)
	PREINIT:
	double lo = 0.0;
	CODE:
	if (items > 1) {
	  lo = (double) SvIV(ST(1));
	  RETVAL = THIS->estimateXA(lo);
	} else {
	  RETVAL = THIS->estimateXA();
	}
	OUTPUT:
	RETVAL

int
TinySVM::Model::getSVnum()
	CODE:
	 RETVAL = THIS->getSVnum();
	OUTPUT:
	 RETVAL

int
TinySVM::Model::size()
	CODE:
	 RETVAL = THIS->getSVnum();
	OUTPUT:
	 RETVAL

void
TinySVM::Model::DESTROY()
