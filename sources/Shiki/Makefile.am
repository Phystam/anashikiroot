SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include $(patsubst %,-I%,$(wildcard ../Reconstruction/*/include)) -I../Core/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -g -Wall -I../Core/include $(patsubst %,-I%,$(wildcard ../Reconstruction/*/include))
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanashiki.la
libanashiki_la_SOURCES = $(SUBSRC1) anashiki_dict.cc
libanashiki_la_DEPENDENCIES = $(SUBOBJ)
libanashiki_la_LIBADD = $(SUBOBJ)
libanashiki_la_LDFLAGS = $(LDFLAGS)

anashiki_dict.cc: $(SUBHDR) anashiki_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^
	@if [ ! -d $(prefix)/lib ]; then \
	mkdir -p $(prefix)/lib; \
	fi
	@if [ -f anashiki_dict_rdict.pcm ]; then \
	cp anashiki_dict_rdict.pcm $(prefix)/lib; \
	fi

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
