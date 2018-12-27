#!/usr/bin/make -f

cflags=-ggdb -Wall -std=c17
ldflags=$(cflags)
ifeq ($(shell uname),Darwin)
	ldflags += -framework OpenGL
endif

all: 01

01: 01.o makefile
	gcc -o $@ $< $(shell pkg-config --libs glew glfw3) $(ldflags)
01.o: 01.c makefile
	gcc -o $@ $< $(shell pkg-config --cflags glew glfw3) $(cflags) -c
