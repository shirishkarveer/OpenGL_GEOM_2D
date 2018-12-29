#!/usr/bin/make -f

cflags=-ggdb -Wall -std=c++17
ldflags=$(cflags)
ccinc=$(shell pkg-config --cflags glew glfw3)
ldinc=$(shell pkg-config --libs glew glfw3)
ifeq ($(shell uname),Darwin)
	ldinc+=-framework OpenGL
endif

all: 04

04: 04.o makefile
	g++ $(ldflags) -o $@ $< $(ldinc)
04.o: 04.cpp makefile
	g++ $(cflags) -o $@ $< $(ccinc) -c
