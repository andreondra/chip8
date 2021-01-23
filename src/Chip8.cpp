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

Chip8::Chip8() 
    : randGenerator(std::chrono::system_clock::now().time_since_epoch().count()){

    memcpy(memory, font, sizeof(uint8_t) * SIZE_FONT);
    randByte = std::uniform_int_distribution<uint8_t>(0, 255);
}

void Chip8::Cycle(){

    uint16_t hashed;

    opcode = (memory[pc] << 8) | memory[pc + 1];  
    pc += 2;
    
    switch(opcode & 0xF000){

        case 0x8000:
        case 0x0000:
            hashed = opcode & 0xF00F;
            break;
        
        case 0xE000:
        case 0xF000:
            hashed = opcode & 0xF0FF;
            break;

        default:
            hashed = opcode & 0xF000;
            break;
    }

    switch(hashed){

        case 0x1000: OP_1NNN(); break;
        case 0x2000: OP_2NNN(); break;
        case 0x3000: OP_3XKK(); break;
        case 0x4000: OP_4XKK(); break;
        case 0x5000: OP_5XY0(); break;
        case 0x6000: OP_6XKK(); break;
        case 0x7000: OP_7XKK(); break;
        case 0x8000: OP_8XY0(); break;
        case 0x8001: OP_8XY1(); break;
        case 0x8002: OP_8XY2(); break;
        case 0x8003: OP_8XY3(); break;
        case 0x8004: OP_8XY4(); break;
        case 0x8005: OP_8XY5(); break;
        case 0x8006: OP_8XY6(); break;
        case 0x8007: OP_8XY7(); break;
        case 0x800E: OP_8XYE(); break;
        case 0x0000: OP_00E0(); break;
        case 0x000E: OP_00EE(); break;
        case 0xE0A1: OP_EXA1(); break;
        case 0xE09E: OP_EX9E(); break;
        case 0xF007: OP_FX07(); break;
        case 0xF00A: OP_FX0A(); break;
        case 0xF015: OP_FX15(); break;
        case 0xF018: OP_FX18(); break;
        case 0xF01E: OP_FX1E(); break;
        case 0xF029: OP_FX29(); break;
        case 0xF033: OP_FX33(); break;
        case 0xF055: OP_FX55(); break;
        case 0xF065: OP_FX65(); break;
    }

    if(delayTimer > 0)
        delayTimer--;

    if(soundTimer > 0)
        soundTimer--;
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

void Chip8::OP_8XYE(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;

    if(registers[Vx] & 0x80)
        registers[0xF] = 1;
    else
        registers[0xF] = 0;

    registers[Vx] <<= 1;
}

void Chip8::OP_9XY0(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;

    if(registers[Vx] != registers[Vy])
        pc += 2;    
}

void Chip8::OP_ANNN(){

    index = (opcode & 0x0FFF);
}

void Chip8::OP_BNNN(){

    pc = registers[0] + (opcode & 0x0FFF);
}

void Chip8::OP_CXKK(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t kk = (opcode & 0x00FF);

    registers[Vx] = randByte(randGenerator) & kk;
}

void Chip8::OP_DXYN(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    uint8_t h = (opcode & 0x000F);

    uint8_t x = registers[Vx] % SCREEN_WIDTH;
    uint8_t y = registers[Vy] % SCREEN_HEIGHT;

    registers[0xF] = 0;

    for(uint8_t i = 0; i < h; i++){

        uint8_t sprite = memory[index + i];

        for(uint8_t j = 0; j < 8; j++){

            uint8_t currentPixel = sprite & (0x80 >> j);
            uint32_t *screenPixel = &display[(i + y) * SCREEN_WIDTH + (x + j)];

            if(currentPixel){

                if(*screenPixel == 0xFFFFFFFF)
                    registers[0xF] = 1;

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::OP_EX9E(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t key = registers[Vx];

    if(keypad[key])
        pc += 2;
}

void Chip8::OP_EXA1(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t key = registers[Vx];

    if(!keypad[key])
        pc += 2;
}

void Chip8::OP_FX07(){

    registers[(opcode & 0x0F00) >> 8] = delayTimer;
}

void Chip8::OP_FX0A(){

    bool pressed = 0;

    for(int i = 0; i < 16; i++){

        if(keypad[i]){
            registers[(opcode & 0x0F00) >> 8] = i;
            pressed = 1;
            break;
        }
    }

    if(!pressed)
        pc -= 2;
}

void Chip8::OP_FX15(){

    delayTimer = registers[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_FX18(){

    soundTimer = registers[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_FX1E(){

    index += registers[(opcode & 0x0F00) >> 8];
}

void Chip8::OP_FX29(){

    uint8_t digit = registers[(opcode & 0x0F00) >> 8];

    index = ADR_FONT + (digit * 5);
}

void Chip8::OP_FX33(){

    uint8_t number = registers[(opcode & 0x0F00) >> 8];

    memory[index + 2] = number % 10;
    number /= 10;

    memory[index + 1] = number % 10;
    number /= 10;

    memory[index] = number % 10;
}

void Chip8::OP_FX55(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;

    for(uint8_t i = 0; i <= Vx; i++)
        memory[index + i] = registers[i];
}

void Chip8::OP_FX65(){

    uint8_t Vx = (opcode & 0x0F00) >> 8;

    for(uint8_t i = 0; i <= Vx; i++)
        registers[i] = memory[index + i];
}