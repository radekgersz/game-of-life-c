#include <SDL2/SDL.h>
#include <stdio.h>
#include "graphics.h"

int WINDOW_HEIGHT = 1000;
int WINDOW_WIDTH = 1000;
int GRID_HEIGHT = 200;
int GRID_WIDTH = 200;

int readInput(char filename[], int* grid){
    FILE *fptr;
    fptr = fopen(filename, "r");
    int row, col;
    if (fptr) {
        while (fscanf(fptr, "%d %d", &row, &col) == 2) {
        if (row >= GRID_HEIGHT || col >= GRID_WIDTH){
            printf("invalid cells positioning \n");
            return 1;
        }
        grid[row*GRID_WIDTH + col] = 1;
    }
    fclose(fptr);
    }
    return 0;
}


int main(int argc, char** argv) {
    if (argc != 4){
        printf("Usage: ./game <filename> <num_steps> <graphics_on>\n");
        return -1;
    }    
    int* grid = calloc(GRID_HEIGHT*GRID_WIDTH,sizeof(int));
    if (!grid) return 1;
    char *filename = argv[1];
    // const size_t nSteps = strtoul(argv[2], NULL, 10);
    const int graphicsOn = atoi(argv[3]);
    if (readInput(filename, grid) != 0){
        printf("failed to read input\n");
        return 1;

    }

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
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        free(grid);
        return 1;
    }

    int cellWidth = WINDOW_WIDTH / GRID_WIDTH;
    int cellHeight = WINDOW_HEIGHT / GRID_HEIGHT;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int row = 0; row < GRID_HEIGHT; row++) {
        for (int col = 0; col < GRID_WIDTH; col++) {
            
            if (grid[row * GRID_WIDTH + col] == 1) {
                SDL_Rect cell;
                cell.x = col * cellWidth;
                cell.y = row * cellHeight;
                cell.w = cellWidth > 0 ? cellWidth : 1;
                cell.h = cellHeight > 0 ? cellHeight : 1;
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    SDL_DestroyWindow(win);
    SDL_Quit();
    free(grid);
    return 0;
}
