#include <stdio.h>
#include "graphics.h"
#include <SDL2/SDL.h>


void drawWindow(windowHeight,window){
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