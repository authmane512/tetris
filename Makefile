all: main.o array.o various.o
	gcc -lm `sdl2-config --cflags --libs` main.o array.o various.o -o tetris

main.o: main.c array.h various.h
	gcc -DTYPE=int -c `sdl2-config --cflags --libs` main.c -o main.o

array.o: array.c array.h
	gcc -c -DTYPE=int array.c -o array.o

various.o: various.c various.h
	gcc -c -lm various.c -o various.o