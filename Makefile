default:
	g++ -Wall -pedantic -g src/main.cpp src/Chip8.cpp src/Platform.cpp -o chip8emu -lSDL2 