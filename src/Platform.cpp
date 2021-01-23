#include "Platform.hpp"

Platform::Platform(const void *screenBuffer, uint8_t *keypad, const char *title, int scale){

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, 0, 0, SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH * scale, SCREEN_HEIGHT * scale);
}

Platform::~Platform(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

