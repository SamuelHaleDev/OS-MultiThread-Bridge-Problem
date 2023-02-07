CFLAGS= -g
LDFLAGS= -pthread
CC=g++

all: boundedBuffer condvar1

# To make an executable
boundedBuffer: boundedBuffer.o 
	$(CC) $(LDFLAGS) -o boundedBuffer boundedBuffer.o

condvar1: condvar1.o 
	$(CC) $(LDFLAGS) -o condvar1 condvar1.o
 
# To make an object from source
.c.o:
	$(CC) $(CFLAGS) -c $*.c

# clean out the dross
clean:
	-rm boundedBuffer condvar1 *.o

