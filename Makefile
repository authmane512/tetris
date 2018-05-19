.PHONY: clean

.SUFFIXES:

CC = gcc
CFLAGS = -lm -Wall -Wextra -pedantic -fno-common -fno-builtin -std=c89
SDL2FLAGS = $(shell sdl2-config --cflags --libs)

all: main.o events.o load_graphics.o array.o various.o
	$(CC) -lm $(SDL2FLAGS) main.o events.o load_graphics.o array.o various.o -o tetris

main.o: main.c main.h sdl.h events.h load_graphics.h array.h various.h
	$(CC) $(CFLAGS) -c $(SDL2FLAGS) main.c -o main.o

events.o: main.h sdl.h platforms/desktop/events.c
	$(CC) $(CFLAGS) -c platforms/desktop/events.c -o events.o

load_graphics.o: sdl.h platforms/desktop/load_graphics.c
	$(CC) $(CFLAGS) -c platforms/desktop/load_graphics.c -o load_graphics.o

array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c -o array.o

various.o: various.c various.h
	$(CC) $(CFLAGS) -c -lm various.c -o various.o

clean:
	rm -rf *.o
