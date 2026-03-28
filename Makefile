main: main.c map.o
	cc main.c map.o -o main

map.o: map.h map.c
	cc -c map.c -o map.o

clean:
	rm main map.o
