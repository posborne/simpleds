BUILDDIR = build
TESTDIR = test
SRCDIR = src
OBJS = $(BUILDDIR)/list.o
CC = gcc
DEBUG = -g
VPATH = $(SRCDIR):$(TESTDIR)
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: datastructures

$(BUILDDIR)/%.o: %.c %.h
	$(CC) $(CFLAGS) $< -o $@

datastructures: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o datastructures

tests: datastructures

tar:
	tar czvf datastructures.tar.gz src/*.h src/*.c makefile

clean:
	\rm -f build/*.o src/*~ datastructures