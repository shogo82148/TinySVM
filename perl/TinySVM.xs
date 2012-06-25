#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "tinysvm.h"

/* $Id: TinySVM.xs,v 1.10 2000/12/06 14:52:51 taku-ku Exp $; */

MODULE = TinySVM		PACKAGE = Example

Example *
Example::new(f = NO_INIT)
	char *f;
	PREINIT:
   	Example *e;
	CODE:
	 e = new Example();
	 if (items > 1) e->read(f);
	 RETVAL = e;
	OUTPUT:
	RETVAL

int
Example::read(f, offset = NO_INIT)
	char *f
	int offset;
	CODE:
	  if (items > 2) RETVAL = THIS->read(f,"r",offset);
	  else           RETVAL = THIS->read(f,"r",0);
	OUTPUT:
	RETVAL

int
Example::write(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"w");
	OUTPUT:
	RETVAL

int
Example::append(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"a");
	OUTPUT:
	RETVAL

Model*
Example::learn(opt = NO_INIT)
	char *opt
	PREINIT:
	 Param p;
	 char *CLASS = "Model";
	CODE:
	  if (items > 1) p.set(opt);
	  RETVAL = THIS->learn(p);
	OUTPUT:
	RETVAL

int
Example::add(...)
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

double
Example::getY(i)
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
Example::getX(i)
        int i
	PREINIT:
	feature_node *f;
	char tmp[256];
	PPCODE:
	 if (i >= 0 && i < THIS->size()) {
	   f =  THIS->x[i];
	   for (int j = 0; f[j].index != -1; j++) {
             sprintf(tmp,"%d:%g",f[j].index,f[j].value);
	     PUSHs(sv_2mortal(newSVpv(tmp,strlen(tmp))));
	   }
	  } else {
	   warn( "Example::getX() -- index is out of range" );
           XSRETURN_UNDEF;
          }

int
Example::size()
	CODE:
	 RETVAL = THIS->size();
	OUTPUT:
	 RETVAL

int
Example::clear()
	CODE:
	 RETVAL = THIS->clear();
	OUTPUT:
	 RETVAL

void
Example::DESTROY()


MODULE = TinySVM		PACKAGE = Model

Model *
Model::new(f = NO_INIT)
	char *f;
	PREINIT:
   	Model *m;
	CODE:
	 m = new Model();
	 if (items > 1) m->read(f);
	 RETVAL = m;
	OUTPUT:
	RETVAL

int
Model::read(f, offset = NO_INIT)
	char *f
	int offset;
	CODE:
	  if (items > 2) RETVAL = THIS->read(f,"r",offset);
	  else           RETVAL = THIS->read(f,"r",0); 
	OUTPUT:
	RETVAL

int
Model::write(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"w");
	OUTPUT:
	RETVAL

int
Model::append(f)
	char *f;
	CODE:
	RETVAL = THIS->write(f,"a");
	OUTPUT:
	RETVAL

int
Model::writeSVindex(f)
	char *f;
	CODE:
	RETVAL = THIS->writeSVindex(f,"w");
	OUTPUT:
	RETVAL

int
Model::appendSVindex(f)
	char *f;
	CODE:
	RETVAL = THIS->writeSVindex(f,"a");
	OUTPUT:
	RETVAL

double
Model::getY(i)
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
Model::getAlpha(i)
	int i;
	CODE:
	 if (i >= 0 && i < THIS->size()) 
	   RETVAL = THIS->y[i];
         else  {
	   warn( "Model::getAlpha() -- index is out of range" );
           XSRETURN_UNDEF;
         }
	OUTPUT:
	 RETVAL

void
Model::getX(i)
        int i
	PREINIT:
	feature_node *f;
	char tmp[256];
	int j;
	PPCODE:
	 if (i >= 0 && i < THIS->size()) {
	   f =  THIS->x[i];
	   for (j = 0; f[j].index != -1; j++) {
             sprintf(tmp,"%d:%g",f[j].index,f[j].value);
	     PUSHs(sv_2mortal(newSVpv(tmp,strlen(tmp))));
	   }
	  } else {
	   warn( "Example::getX() -- index is out of range" );
           XSRETURN_UNDEF;
          }

double
Model::classify(x)
        char *x
        CODE:
        RETVAL = THIS->classify(x);
        OUTPUT:
        RETVAL

double
Model::classify_list(...)
	PREINIT:
	int i;
	CODE:
	 struct feature_node *node  = 
	   (struct feature_node *)malloc(sizeof(struct feature_node) * (items + 1));
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
Model::clear()
	CODE:
	 RETVAL = THIS->clear();
	OUTPUT:
	 RETVAL

double
Model::estimateMargin()
	CODE:
	 RETVAL = THIS->estimateMargin();
	OUTPUT:
	 RETVAL
	
double
Model::estimateVC()
	CODE:
	 RETVAL = THIS->estimateVC();
	OUTPUT:
	 RETVAL

int
Model::getSVnum()
	CODE:
	 RETVAL = THIS->getSVnum();
	OUTPUT:
	 RETVAL

int
Model::size()
	CODE:
	 RETVAL = THIS->getSVnum();
	OUTPUT:
	 RETVAL

void
Model::DESTROY()

