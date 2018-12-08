cflags=-ggdb -Wall -std=c++17

all: 01

01: 01.o makefile
	g++ $(cflags) -o $@ $< $(shell pkg-config --libs glew glfw3 glm)
01.o: 01.cpp makefile
	g++ $(cflags) -o $@ $< $(shell pkg-config --cflags glew glfw3 glm) -c

# The following have pkg-config entries:
# x11 xext glu glfw3 glew glm
