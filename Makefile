################################################################################

CXXFLAGS	= -pipe -march=native -m64 -O3 -funroll-loops \
		  -finline-functions -finline-functions-called-once

LDLIBS		= -lX11

REASON		= @if [ -f $@ ]; then echo "[$@: $?]"; else echo "[$@]"; fi

%.o: %.cc
	$(REASON)
	$(COMPILE.cc) $< $(OUTPUT_OPTION)

%.rx: %.rc rcas
	$(REASON)
	rcas $< > $@

################################################################################

all: xmars mars rx

xmars: xmars.o loader.o visualizer.o graphix.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

mars: mars.o loader.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

REDCODE = $(wildcard *.rc)
REDEXEC = $(patsubst %.rc, %.rx, $(REDCODE))

rx: $(REDEXEC)

################################################################################

ckgraphix: ckgraphix.o graphix.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

ckvisualizer: ckvisualizer.o visualizer.o graphix.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

cknumber: cknumber.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

ckaddress: ckaddress.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

ckmemory: ckmemory.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

ckscheduler: ckscheduler.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

ckprocessor: ckprocessor.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

ckloader: ckloader.o loader.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

ckmachine: ckmachine.o
	$(REASON)
	$(LINK.cc) -o $@ $^ $(LDLIBS)

test: cknumber ckaddress ckmemory ckscheduler ckprocessor ckloader ckloader.rx
	$(REASON)
	cknumber
	ckaddress
	ckmemory
	ckscheduler
	ckloader ckloader.rx
	ckprocessor

################################################################################

init:
	$(REASON)
	chmod +x rcas turnament evaluate turnament3 evaluate3

deps depend:
	$(REASON)
	$(CC) -MM $(CFLAGS) *.cc > deps

clean:
	$(REASON)
	$(RM) *.o ckgraphix ckvisualizer cknumber ckaddress ckmemory \
	  ckscheduler ckprocessor ckloader ckloader.rx ckmachine

distclean: clean
	$(REASON)
	$(RM) xmars mars *.rx deps *~ core

-include deps

################################################################################
