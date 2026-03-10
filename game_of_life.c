#include <SDL2/SDL.h>
#include <stdio.h>
#include "graphics.h"

int WINDOW_HEIGHT = 1000;
int WINDOW_WIDTH = 800;
int GRID_HEIGHT = 1000;
int GRID_WIDTH = 1000;
void readInput(char filename[]){
    FILE *fptr;
    fptr = fopen(filename, "r");
    int row, col;
    if (fptr) {
        while (fscanf(fptr, "%d %d", &row, &col) == 2) {
        printf("Read cell: %d %d\n", row, col);
    }
    }
    fclose(fptr);
}


int main(int argc, char** argv) {
    if (argc != 4){
        printf("Usage: ./game <filename> <num_steps> <graphics_on>\n");
        return -1;
    }    
    int* grid = malloc(GRID_HEIGHT*GRID_WIDTH*sizeof(int));
    const char *filename = argv[1];
    // const size_t nSteps = strtoul(argv[2], NULL, 10);
    // const int graphicsOn = atoi(argv[3]);
    readInput(filename);
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_HEIGHT, WINDOW_WIDTH,
        SDL_WINDOW_SHOWN
    );

    if (!win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Delay(2000);

    SDL_DestroyWindow(win);
    SDL_Quit();
    free(grid);
    return 0;
}


