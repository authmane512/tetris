.PHONY: clean

.SUFFIXES:

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fno-common -fno-builtin -std=c89
SDL2FLAGS = `sdl2-config --cflags --libs`

all: main.o events.o load_graphics.o debugging.o
	$(CC) main.o events.o load_graphics.o debugging.o -o tetris $(SDL2FLAGS) -lm

main.o: main.c main.h sdl.h events.h load_graphics.h debugging.h
	$(CC) $(CFLAGS) main.c -o main.o -c $(SDL2FLAGS) -lm

events.o: main.h sdl.h platforms/desktop/events.c
	$(CC) $(CFLAGS) -c platforms/desktop/events.c -o events.o -lm

load_graphics.o: sdl.h platforms/desktop/load_graphics.c
	$(CC) $(CFLAGS) -c platforms/desktop/load_graphics.c -o load_graphics.o -lm

debugging.o: debugging.c debugging.h
	$(CC) $(CFLAGS) -c debugging.c -o debugging.o -lm

clean:
	rm -rf *.o
