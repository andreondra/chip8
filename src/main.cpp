#include <chrono>
#include <iostream>
#include "Platform.hpp"
#include "Chip8.hpp"

int main(int argc, char** argv){
    
    if(argc != 4){
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    int scale = std::stoi(argv[1]);
    int cycle = std::stoi(argv[2]);
    char const *rom = argv[3];

    Chip8 chip8;
    Platform platform(chip8.display, chip8.keypad, "CHIP-8 Emulator", scale);

    int loadStatus = chip8.loadROM(rom);
    if(loadStatus != 0){
        std::cerr << "Can't load " << argv[3] << "\n";
        std::cerr << "Error: " << loadStatus << "\n";
        std::exit(EXIT_FAILURE);
    };

    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    while(!quit){

        quit = platform.GetInput();
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

        if (dt > cycle){

            lastCycleTime = currentTime;
            chip8.Cycle();
            platform.Update();
        }
    }

    return 0;
}