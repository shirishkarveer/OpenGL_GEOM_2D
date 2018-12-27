#!/usr/bin/make -f

cflags=-ggdb -Wall -std=c17
ldflags=$(cflags)
ccinc=$(shell pkg-config --cflags glew glfw3)
ldinc=$(shell pkg-config --libs glew glfw3)
ifeq ($(shell uname),Darwin)
	ldinc+=-framework OpenGL
endif

all: 02

02: 02.o makefile
	gcc $(ldflags) -o $@ $< $(ldinc)
02.o: 02.c makefile
	gcc $(cflags) -o $@ $< $(ccinc) -c
