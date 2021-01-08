#ifndef __GSS_CHIP8_H__
#define __GSS_CHIP8_H__

#include <cstdint>
#include <random>

/**
 * CHIP-8 interpreter.
 * @author Ondrej Golasowski
 */
class Chip8{

    private:
        static const uint16_t ADR_START = 0x200;
        static const uint8_t ADR_FONT = 0x50;
        static const uint8_t SIZE_FONT = 80;
        static const uint16_t SIZE_SCREEN = 2048;
        static const uint8_t font[SIZE_FONT];

        typedef enum returnStatus {
            RS_OK,
            RS_ROM_TOOBIG,
            RS_ROM_IOERROR   
        } returnStatus_t;
        
        uint8_t registers[16];
        uint8_t memory[4096];
        uint16_t index;
        uint16_t pc;
        uint16_t stack[16];
        uint8_t stackPointer;
        uint8_t delayTimer;
        uint8_t soundTimer;
        uint8_t keypad[16];
        uint32_t display[SIZE_SCREEN];
        uint16_t opcode;

        std::default_random_engine randGenerator;
        std::uniform_int_distribution<uint8_t> randByte;

        returnStatus_t loadROM(const char *filename);

        //Opcodes.
        void Chip8::OP_00E0(); //CLS
        void Chip8::OP_00EE(); //RET
        void Chip8::OP_1NNN(); //JP addr
        void Chip8::OP_2NNN(); //CALL addr
        void Chip8::OP_3XKK(); //SE Vx, byte
        void Chip8::OP_4XKK(); //SNE Vx, byte
        void Chip8::OP_5XY0(); //SE Vx, Vy
        void Chip8::OP_6XKK(); //LD Vx, byte
        void Chip8::OP_7XKK(); //ADD Vx, byte
        void Chip8::OP_8XY0(); //LD Vx, Vy
        void Chip8::OP_8XY1(); //OR Vx, Vy
        void Chip8::OP_8XY2(); //AND Vx, Vy
        void Chip8::OP_8XY3(); //XOR Vx, Vy
        void Chip8::OP_8XY4(); //ADD Vx, Vy
        void Chip8::OP_8XY5(); //SUB Vx, Vy
        void Chip8::OP_8XY6(); //SHR Vx
        void Chip8::OP_8XY7(); //SUBN Vx, Vy

    public:
        Chip8();
};

#endif
