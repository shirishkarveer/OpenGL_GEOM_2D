#!/usr/bin/make -f

cflags=-ggdb -Wall -std=c17

all: 01

01: 01.o makefile
	gcc $(cflags) -o $@ $< $(shell pkg-config --libs glew glfw3)
01.o: 01.c makefile
	gcc $(cflags) -o $@ $< $(shell pkg-config --cflags glew glfw3) -c
