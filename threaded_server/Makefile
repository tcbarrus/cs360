# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

SERVER=				server.o

OBJS =			$(SERVER)

LIBS=

CCFLAGS= -g -std=c++0x -lpthread -pthread

all:	server

server:$(SERVER)
	$(CXX) -o server $(SERVER) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) server

include	$(OBJS:.o)
