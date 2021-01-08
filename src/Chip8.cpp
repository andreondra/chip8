#include "Chip8.hpp"
#include <cstdio>
#include <cstring>
#include <chrono>

const uint8_t Chip8::font[SIZE_FONT] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

Chip8::Chip8() : randGenerator(std::chrono::system_clock::now().time_since_epoch().count()){

    memcpy(memory, font, sizeof(uint8_t) * SIZE_FONT);
    randByte = std::uniform_int_distribution<uint8_t>(0, 255);
}

Chip8::returnStatus_t Chip8::loadROM(const char *filename){

    FILE *rom;
    size_t readStatus;
    size_t dataSize;

    if(NULL == (rom = fopen(filename, "rb"))){
        return RS_ROM_IOERROR;
    }

    readStatus = fread(memory + ADR_START, sizeof(uint8_t), 4096, rom);

    if(readStatus != 4096){
        return RS_ROM_IOERROR;
    }

    fclose(rom);
    rom = NULL;
    
    return RS_OK;
}

void Chip8::OP_00E0(){
    memset(display, 0, SIZE_SCREEN);
}

void Chip8::OP_00EE(){
    pc = stack[--stackPointer];
}

void Chip8::OP_1NNN(){
    pc = opcode & 0x0FFF;
}

void Chip8::OP_2NNN(){

    stack[stackPointer] = pc;
    stackPointer++;
    pc = opcode & 0x0FFF;
}

void Chip8::OP_3XKK(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;

    if(registers[Vx] == byte)
        pc += 2;
}

void Chip8::OP_4XKK(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t byte = opcode & 0x00FF;

    if(registers[Vx] != byte)
        pc += 2;
}

void Chip8::OP_5XY0(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    if(registers[Vx] == registers[Vy])
        pc += 2;
}

void Chip8::OP_6XKK(){
    registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
}

void Chip8::OP_7XKK(){
    registers[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
}

void Chip8::OP_8XY0(){
    registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00F0) >> 4;
}

void Chip8::OP_8XY1(){
    registers[(opcode & 0x0F00) >> 8] |= (opcode & 0x00F0) >> 4;
}

void Chip8::OP_8XY2(){
    registers[(opcode & 0x0F00) >> 8] &= (opcode & 0x00F0) >> 4;
}

void Chip8::OP_8XY3(){
    registers[(opcode & 0x0F00) >> 8] ^= (opcode & 0x00F0) >> 4;
}

void Chip8::OP_8XY4(){

    uint16_t sum = registers[(opcode & 0x0F00) >> 8] + registers[(opcode & 0x00F0) >> 4];

    if(sum > 255)
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[(opcode & 0x0F00) >> 8] = sum & 0xFF;
}

void Chip8::OP_8XY5(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    if(registers[Vx] > registers[Vy])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[Vx] -= registers[Vy];
}

void Chip8::OP_8XY6(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;

    registers[0xF] = registers[Vx] & 0x1;

    registers[Vx] >>= 1;
}

void Chip8::OP_8XY7(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    if(registers[Vy] > registers[Vx])
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[Vx] = registers[Vy] - registers[Vx];
}