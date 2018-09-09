.PHONY: clean

.SUFFIXES:

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fno-common -fno-builtin -std=c89
SDL2FLAGS = `sdl2-config --cflags --libs`

all: main.o events.o load_graphics.o array.o various.o
	$(CC) main.o events.o load_graphics.o array.o various.o -o tetris $(SDL2FLAGS) -lm

main.o: main.c main.h sdl.h events.h load_graphics.h array.h various.h
	$(CC) $(CFLAGS) main.c -o main.o -c $(SDL2FLAGS) -lm

events.o: main.h sdl.h platforms/desktop/events.c
	$(CC) $(CFLAGS) -c platforms/desktop/events.c -o events.o -lm

load_graphics.o: sdl.h platforms/desktop/load_graphics.c
	$(CC) $(CFLAGS) -c platforms/desktop/load_graphics.c -o load_graphics.o -lm

array.o: array.c array.h
	$(CC) $(CFLAGS) -c array.c -o array.o -lm

various.o: various.c various.h
	$(CC) $(CFLAGS) -c -lm various.c -o various.o -lm

clean:
	rm -rf *.o
