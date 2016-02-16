# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

SERVER=				server.o
CLIENT=				download.o
OBJS =			$(SERVER) $(CLIENT)

LIBS=

CCFLAGS= -g -std=c++11

all:	client

client:$(CLIENT)
	$(CXX) -o download $(CLIENT) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) download


include	$(OBJS:.o)
