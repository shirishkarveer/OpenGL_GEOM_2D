cflags=-ggdb -Wall -std=c++17

all: 01

01: 01.o makefile
	g++ $(cflags) $(shell pkg-config --libs glew glfw3 glm) -o $@ $<
	# -L:libglfw.so.3
01.o: 01.cpp makefile
	g++ $(cflags) $(shell pkg-config --cflags glew glfw3 glm) -o $@ $< -c

# The following have pkg-config entries:
# x11 xext glu glfw3 glew glm
