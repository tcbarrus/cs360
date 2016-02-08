# Makefile

CXX=			g++ $(CCFLAGS)

EPOLL=				client.o

OBJS =			$(EPOLL)

LIBS=

CCFLAGS= -g

all:	epoll

epoll:$(EPOLL)
	$(CXX) -o client $(EPOLL) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) client

include	$(OBJS:.o)