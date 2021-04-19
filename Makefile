CC=g++

CFLAGS=-lGL -lX11 -lglfw -lXi -ldl -Wall -I ./include/ -I /usr/include/

triangle: triangle.cpp
	$(CC) $(CFLAGS) -o triangle triangle.cpp src/glad.c
	./triangle
