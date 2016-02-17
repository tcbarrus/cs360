# Makefile for echo client and server

CXX=			g++ $(CCFLAGS)

CLIENT=				client.o
OBJS =			$(CLIENT)

LIBS=

CCFLAGS= -g -std=c++11

all:	client

client:$(CLIENT)
	$(CXX) -o webtest $(CLIENT) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o)

realclean:
	rm -f $(OBJS) $(OBJS:.o) webtest


include	$(OBJS:.o)
