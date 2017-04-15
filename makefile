CFLAGS = -Wall

SERVER = simpled
CLIENTS = ssSet ssGet ssDigest ssRun

all: $(SERVER) $(CLIENTS)

csapp.h:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/include/csapp.h

csapp.c:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/src/csapp.c

csapp.o: csapp.h csapp.c

clients: ssSet ssGet ssDigest ssRun

simpled: simpled.c csapp.o 
	gcc $(CFLAGS) simpled.c csapp.o -lpthread -o simpled

ssSet: ssSet.c csapp.o 
	gcc $(CFLAGS) ssSet.c csapp.o -lpthread -o ssSet

ssGet: ssGet.c csapp.o 
	gcc $(CFLAGS) ssGet.c csapp.o -lpthread -o ssGet

ssDigest: ssDigest.c csapp.o 
	gcc $(CFLAGS) ssDigest.c csapp.o -lpthread -o ssDigest

ssRun: ssRun.c csapp.o 
	gcc $(CFLAGS) ssRun.c csapp.o -lpthread -o ssRun

.PHONY: clean
clean:
	/bin/rm -rf csapp.h csapp.c *.o simpled ssSet ssGet ssDigest ssRun
