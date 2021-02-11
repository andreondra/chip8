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
        static const uint8_t SCREEN_WIDTH = 64;
        static const uint8_t SCREEN_HEIGHT = 32;
        static const uint8_t font[SIZE_FONT];

        typedef enum returnStatus {
            RS_OK,
            RS_ROM_TOOBIG,
            RS_ROM_IOERROR   
        } returnStatus_t;
        
        uint8_t registers[16]{};
        uint8_t memory[4096]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16]{};
        uint8_t stackPointer{};
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint16_t opcode{};

        std::default_random_engine randGenerator;
        std::uniform_int_distribution<uint8_t> randByte;

        //Opcodes.
        void OP_00E0(); //CLS
        void OP_00EE(); //RET
        void OP_1NNN(); //JP addr
        void OP_2NNN(); //CALL addr
        void OP_3XKK(); //SE Vx, byte
        void OP_4XKK(); //SNE Vx, byte
        void OP_5XY0(); //SE Vx, Vy
        void OP_6XKK(); //LD Vx, byte
        void OP_7XKK(); //ADD Vx, byte
        void OP_8XY0(); //LD Vx, Vy
        void OP_8XY1(); //OR Vx, Vy
        void OP_8XY2(); //AND Vx, Vy
        void OP_8XY3(); //XOR Vx, Vy
        void OP_8XY4(); //ADD Vx, Vy
        void OP_8XY5(); //SUB Vx, Vy
        void OP_8XY6(); //SHR Vx
        void OP_8XY7(); //SUBN Vx, Vy
        void OP_8XYE(); //SHL Vx {, Vy}
        void OP_9XY0(); //SNE Vx, Vy
        void OP_ANNN(); //LD I, addr
        void OP_BNNN(); //JP V0, addr
        void OP_CXKK(); //RND Vx, byte
        void OP_DXYN(); //DRW Vx, Vy, nibble
        void OP_EX9E(); //SKP Vx
        void OP_EXA1(); //SKNP Vx
        void OP_FX07(); //LD Vx, DT
        void OP_FX0A(); //LD Vx, K
        void OP_FX15(); //LD DT, Vx
        void OP_FX18(); //LD ST, Vx
        void OP_FX1E(); //ADD I, Vx
        void OP_FX29(); //LD F, Vx
        void OP_FX33(); //LD B, Vx
        void OP_FX55(); //LD [I], Vx
        void OP_FX65(); //LD Vx, [I]

    public:
        Chip8();
        void Cycle();
        returnStatus_t loadROM(const char *filename);
        uint8_t keypad[16]{};
        uint32_t display[SIZE_SCREEN]{};
};

#endif
