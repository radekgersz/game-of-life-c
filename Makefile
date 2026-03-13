CFLAGS  += -Wall -Wextra -O3 -fopenmp -ffast-math -march=native $(shell sdl2-config --cflags)
LDFLAGS += -fopenmp $(shell sdl2-config --libs)

game: game_of_life.o
	gcc game_of_life.o -o game $(LDFLAGS)

game_of_life.o: game_of_life.c
	gcc $(CFLAGS) -c game_of_life.c

clean:
	rm -f *.o game