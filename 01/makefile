#!/usr/bin/make -f

cflags=-ggdb -Wall -std=c17
ldflags=$(cflags)
ccinc=$(shell pkg-config --cflags glfw3)
ldinc=$(shell pkg-config --libs glfw3)
ifeq ($(shell uname),Darwin)
	ldinc+=-framework OpenGL
else
	ccinc+=$(shell pkg-config --cflags gl)
	ldinc+=$(shell pkg-config --libs gl)
endif

all: 01

01: 01.o makefile
	gcc $(ldflags) -o $@ $< $(ldinc)
01.o: 01.c makefile
	gcc $(cflags) -o $@ $< $(ccinc) -c
