# Makefile for echo client and server

CXX=			gcc $(CCFLAGS)

SIGINT=				sigint.o

OBJS =			$(SIGINT)

LIBS=

CCFLAGS= -g

all:	sigint

sigint:$(SIGINT)
	$(CXX) -o sigint $(SIGINT) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) sigint

include	$(OBJS:.o)