#!/usr/bin/make -f

cflags=-ggdb -Wall -std=c++17
ldflags=$(cflags)
ccinc=$(shell pkg-config --cflags glew glfw3)
ldinc=$(shell pkg-config --libs glew glfw3)
ifeq ($(shell uname),Darwin)
	ldinc+=-framework OpenGL
endif

all: 03

03: 03.o makefile
	g++ $(ldflags) -o $@ $< $(ldinc)
03.o: 03.cpp makefile
	g++ $(cflags) -o $@ $< $(ccinc) -c
