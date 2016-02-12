# Makefile

CXX=			g++ $(CCFLAGS)

CODE=				code.o

OBJS =			$(CODE)

LIBS=

CCFLAGS= -g

all:	code

code:$(CODE)
	$(CXX) -o code $(CODE) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) code

include	$(OBJS:.o)
