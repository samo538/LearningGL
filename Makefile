CC=gcc
CFLAGS=
LDFLAGS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

PHONY: all

all: main.c
	$(CC) $(CFLAGS) $^ glad.c -o main $(LDFLAGS)

