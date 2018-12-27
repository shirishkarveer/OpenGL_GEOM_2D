#!/usr/bin/make -f

cflags=-ggdb -Wall -std=c17

all: 02

02: 02.o makefile
	gcc $(cflags) -o $@ $< $(shell pkg-config --libs glew glfw3)
02.o: 02.c makefile
	gcc $(cflags) -o $@ $< $(shell pkg-config --cflags glew glfw3) -c
