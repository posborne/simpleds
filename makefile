OBJS = list.o
CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: datastructures

datastructures: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o datastructures

tar:
	tar czvf *.h *.c makefile

clean:
	\rm *.o *~ datastructures