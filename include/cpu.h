
#ifndef CPU_H
    #define CPU_H

    #include <stdint.h>
    #include <stdbool.h>

    #define RAM_MEMORY 4096
    #define MAX_REGISTERS 16
    #define STACK_SIZE 16
    #define SCREEN_SIZE (64 * 32) // Per specs
    #define MAX_INPUTS 16

    #define ROM_START_ADDRESS 0x200 // 512 bytes
    #define ROM_END_ADDRESS  0xFFF //4095 bytes
    
    #define CYCLES_PER_FRAME 10

    /* V register indices -- match CHIP-8 spec naming convention */
    #define V0  0
    #define V1  1
    #define V2  2
    #define V3  3
    #define V4  4
    #define V5  5
    #define V6  6
    #define V7  7
    #define V8  8
    #define V9  9
    #define VA  10
    #define VB  11
    #define VC  12
    #define VD  13
    #define VE  14
    #define VF  15

typedef struct cpu {
    uint8_t memory[RAM_MEMORY];
    uint8_t delay_timer; 
    uint8_t sound_timer;
    uint16_t pc;
    uint8_t sp;
    uint8_t display_buffer[SCREEN_SIZE];
    uint8_t input_pad[MAX_INPUTS];
    uint16_t stack[STACK_SIZE];
    uint8_t v_registers[MAX_REGISTERS]; // V0 -> VF (VF is splecial, it holds a flag for the carry-out for ADD, MULT etc)
    uint16_t index_register; // I : holds the 12bit register address.
    bool halted;
} Cpu;

    int run_cpu(int argc, char *argv[]);

    uint16_t fetch_opcode(Cpu *cpu);
    int decode_opcode(Cpu *cpu, uint16_t opcode);

#endif
