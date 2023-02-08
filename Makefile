CFLAGS= -g
LDFLAGS= -pthread
CC=g++

all: boundedBuffer condvar1 bridge

# To make an executable
boundedBuffer: boundedBuffer.o 
	$(CC) $(LDFLAGS) -o boundedBuffer boundedBuffer.o

condvar1: condvar1.o 
	$(CC) $(LDFLAGS) -o condvar1 condvar1.o

bridge: bridge.o 
	$(CC) $(LDFLAGS) -o bridge bridge.o
 
# To make an object from source
.c.o:
	$(CC) $(CFLAGS) -c $*.c

# clean out the dross
clean:
	-rm boundedBuffer condvar1 bridge *.o

