# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

SERVER=				server.o

SAMPLE=				sample.o

OBJS =			$(SERVER) $(SAMPLE)

LIBS=

CCFLAGS= -g -std=c++0x

all:	server sample

server:$(SERVER)
	$(CXX) -o server $(SERVER) $(LIBS)

sample:$(SAMPLE)
	$(CXX) -o sample $(SAMPLE) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) server sample


include	$(OBJS:.o)