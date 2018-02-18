all: main.o events.o load_graphics.o array.o various.o
	gcc -lm `sdl2-config --cflags --libs` main.o events.o load_graphics.o array.o various.o -o tetris

main.o: main.c main.h sdl.h events.h load_graphics.h array.h various.h
	gcc -c `sdl2-config --cflags --libs` main.c -o main.o

events.o: main.h sdl.h platforms/desktop/events.c
	gcc -c platforms/desktop/events.c -o events.o

load_graphics.o: sdl.h platforms/desktop/load_graphics.c
	gcc -c platforms/desktop/load_graphics.c -o load_graphics.o

array.o: array.c array.h
	gcc -c array.c -o array.o

various.o: various.c various.h
	gcc -c -lm various.c -o various.o