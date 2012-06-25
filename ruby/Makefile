SHELL = /bin/sh

#### Start of system configuration section. ####

srcdir = .
topdir = /usr/lib/ruby/1.6/i386-linux-gnu
hdrdir = /usr/lib/ruby/1.6/i386-linux-gnu
VPATH = $(srcdir)

CC = gcc

CFLAGS   = -fPIC -O2 -march=i386 -mcpu=i686 -fPIC 
CPPFLAGS = -I$(hdrdir) -I/usr/include
CXXFLAGS = $(CFLAGS)
DLDFLAGS =  -L$(topdir) -L/usr/lib  -ltinysvm
LDSHARED = gcc -shared 
LIBPATH = 

RUBY_INSTALL_NAME = ruby
RUBY_SO_NAME = 

prefix = $(DESTDIR)/usr
exec_prefix = $(DESTDIR)/usr
libdir = $(DESTDIR)/usr/lib/ruby/1.6
archdir = $(DESTDIR)/usr/lib/ruby/1.6/i386-linux-gnu
sitelibdir = $(DESTDIR)/usr/lib/ruby/site_ruby/1.6
sitearchdir = $(DESTDIR)/usr/lib/ruby/site_ruby/1.6/i386-linux-gnu

#### End of system configuration section. ####

LOCAL_LIBS =  
LIBS = -L. -l$(RUBY_INSTALL_NAME) -lc
OBJS = TinySVM.o

TARGET = TinySVM
DLLIB = $(TARGET).so

RUBY = ruby
RM = $(RUBY) -r ftools -e 'File::rm_f(*Dir[ARGV.join(" ")])'

EXEEXT = 

all:		$(DLLIB)

clean:;		@$(RM) *.o *.so *.sl *.a $(DLLIB)
		@$(RM) $(TARGET).lib $(TARGET).exp $(TARGET).ilk *.pdb

distclean:	clean
		@$(RM) Makefile extconf.h conftest.*
		@$(RM) core ruby$(EXEEXT) *~

realclean:	distclean

install:	$(archdir)/$(DLLIB)

site-install:	$(sitearchdir)/$(DLLIB)

$(archdir)/$(DLLIB): $(DLLIB)
	@$(RUBY) -r ftools -e 'File::makedirs(*ARGV)' $(libdir) $(archdir)
	@$(RUBY) -r ftools -e 'File::install(ARGV[0], ARGV[1], 0555, true)' $(DLLIB) $(archdir)/$(DLLIB)

$(sitearchdir)/$(DLLIB): $(DLLIB)
	@$(RUBY) -r ftools -e 'File::makedirs(*ARGV)' $(libdir) $(sitearchdir)
	@$(RUBY) -r ftools -e 'File::install(ARGV[0], ARGV[1], 0555, true)' $(DLLIB) $(sitearchdir)/$(DLLIB)


.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
$(DLLIB): $(OBJS)
	$(LDSHARED) $(DLDFLAGS) -o $(DLLIB) $(OBJS) $(LIBS) $(LOCAL_LIBS)
