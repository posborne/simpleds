BUILDDIR = build
TESTDIR = test
SRCDIR = src
CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: scons

scons:
	\scons .

clean:
	\rm -rf src/*.o build/*.o