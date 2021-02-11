#ifndef __GSS_CHIP8PLATFORM_H__
#define __GSS_CHIP8PLATFORM_H__

#include <cstdint>
#include <SDL2/SDL.h>

class Platform{

    private:
        static const uint8_t SCREEN_WIDTH = 64;
        static const uint8_t SCREEN_HEIGHT = 32;
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        SDL_Texture* texture{};
        const void *screenBuffer;
        uint8_t *keypad;

    public:
        Platform(const void *screenBuffer, uint8_t *keypad, const char *title, int scale);
        ~Platform();
        void Update();
        bool GetInput();
};

#endif