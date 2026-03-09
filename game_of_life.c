#include <SDL2/SDL.h>
#include <stdio.h>
#include "graphics.h"
#include "game_of_life.h"

int WINDOW_HEIGHT = 1000;
int WINDOW_WIDTH = 800;
int GRID_HEIGHT = 1000;
int GRID_WIDTH = 1000;


int main(int argc, char** argv) {
    if (argc != 3){
        printf("Usage: ./game <num_steps> <graphics_on>\n");
        return -1;
    }    
    int gameGrid[GRID_HEIGHT*GRID_WIDTH];

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
    return 0;
}


void readInput(char filename[], int* gameGrid){
    FILE *fptr;
    fptr = fopen("filename.txt", "r");

}