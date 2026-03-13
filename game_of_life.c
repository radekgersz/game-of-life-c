#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include "graphics.h"

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000
#define GRID_HEIGHT 2000
#define GRID_WIDTH 2000
#define PADDING (GRID_WIDTH + 2)
int cellWidth;
int cellHeight;

typedef struct 
{
    SDL_Renderer *renderer;
    SDL_Window *win;
} GraphicsData;




int readInput(char filename[], uint8_t* grid){
    FILE *fptr;
    fptr = fopen(filename, "r");
    int row, col;
    if (fptr) {
        while (fscanf(fptr, "%d %d", &row, &col) == 2) {
        if (row >= GRID_HEIGHT || col >= GRID_WIDTH){
            printf("invalid cells positioning \n");
            return 1;
        }
        grid[(row + 1) * PADDING + (col + 1)] = 1;
    }
    fclose(fptr);
    }
    return 0;
}
int writeOutput(){
    return 0;
}
// __attribute__((hot))
void computeNextGeneration(uint8_t* grid, uint8_t* nextGrid){

    #pragma omp parallel for schedule(static)
    for (int y = 1; y <= GRID_HEIGHT; y++) {
        for (int x = 1; x <= GRID_WIDTH; x++) {
            
            // 1D index calculation
            int i = y * PADDING + x;
            
            // 1. Count neighbors (Memory is accessed sequentially where possible)
            uint8_t neighbours = grid[i - PADDING - 1] + grid[i - PADDING] + grid[i - PADDING + 1] +
                          grid[i - 1] + grid[i + 1] +
                          grid[i + PADDING - 1] + grid[i + PADDING] + grid[i + PADDING + 1];
            
            //Applying conway's rules
            nextGrid[i] = (neighbours == 3) | (grid[i] & (neighbours == 2));
        }
    }
}

void drawSimulation(uint8_t* grid, SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // 1. Iterate over the playable area (ignoring the invisible halo)
    for (int y = 1; y <= GRID_HEIGHT; y++) {
        for (int x = 1; x <= GRID_WIDTH; x++) {
            
            // Use the exact same 1D index calculation as your compute function!
            int i = y * PADDING + x;
            
            if (grid[i] == 1) {
                SDL_Rect cell;
                // Subtract 1 so the top-left cell draws at screen coordinate (0,0)
                cell.x = x - 1; 
                cell.y = y - 1;
                cell.w = 1;
                cell.h = 1;
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }
    SDL_RenderPresent(renderer); 
}
uint8_t* performSimulation(uint8_t* grid, uint8_t* nextGrid, const size_t nSteps, const  int graphicsOn, SDL_Renderer *renderer){
    for (size_t i = 0; i < nSteps; i++){
        if (graphicsOn == 1) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return NULL; 
                }
            }
        }

        computeNextGeneration(grid, nextGrid);
        if (graphicsOn == 1){
            drawSimulation(grid,renderer);
            SDL_Delay(20);
        }
        uint8_t* temp = grid;
        grid = nextGrid;      
        nextGrid = temp;
    }
    return grid;
}

int initializeGraphics(GraphicsData* graphicsData){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
    }

    graphicsData->win = SDL_CreateWindow(
        "Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_HEIGHT, WINDOW_WIDTH,
        SDL_WINDOW_SHOWN
    );

    if (!graphicsData->win) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    graphicsData->renderer = SDL_CreateRenderer(graphicsData->win, -1, SDL_RENDERER_ACCELERATED);
    if (!graphicsData->renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(graphicsData->win);
        SDL_Quit();
        return 1;
    }

    SDL_RenderSetLogicalSize(graphicsData->renderer, GRID_WIDTH, GRID_HEIGHT);
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 4){
        printf("Usage: ./game <filename> <num_steps> <graphics_on>\n");
        return -1;
    }   
    //parse initial arguments
    char *filename = argv[1];
    const size_t nSteps = strtoul(argv[2], NULL, 10); 
    const int graphicsOn = atoi(argv[3]);
    uint8_t* grid = calloc(PADDING * (GRID_HEIGHT + 2), sizeof(uint8_t));
    uint8_t* nextGrid = calloc(PADDING * (GRID_HEIGHT + 2), sizeof(uint8_t));
    GraphicsData* graphicsData = malloc(sizeof(GraphicsData));
    cellWidth = WINDOW_WIDTH / GRID_WIDTH;
    cellHeight = WINDOW_HEIGHT / GRID_HEIGHT;

    //input reading
    if (readInput(filename, grid) != 0){
        printf("failed to read input\n");
        return 1;
    }
    if (graphicsOn == 1){
        if (initializeGraphics(graphicsData) != 0){
            free(grid);
            free(nextGrid);
            return 1;
        }
    }
    performSimulation(grid, nextGrid, nSteps, graphicsOn,graphicsData->renderer);

    //writing output
    writeOutput();

    if (graphicsOn == 1){
        SDL_DestroyWindow(graphicsData->win);
        SDL_Quit();
    }
    free(grid);
    free(nextGrid);
    return 0;
}

