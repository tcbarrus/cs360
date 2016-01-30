# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

SERVER=				simple.o

OBJS =			$(SERVER)

LIBS=

CCFLAGS= -g 
#-std=c++0x

all:	server

server:$(SERVER)
	$(CXX) -o simple $(SERVER) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) server


include	$(OBJS:.o)