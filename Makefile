project: main.o f.o g.o
	cc main.o snake.o -o try
main.o: main.c snake.h
	cc -Wall -c main.c
snake.o: snake.c snake.h 
	cc -Wall -c snake.c -lm



