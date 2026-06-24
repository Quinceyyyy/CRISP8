
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>


void exec_or(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    cpu->v_registers[reg_X] |= cpu->v_registers[reg_Y];
}

void exec_and(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    cpu->v_registers[reg_X] &= cpu->v_registers[reg_Y];
}

void exec_xor(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    cpu->v_registers[reg_X] ^= cpu->v_registers[reg_Y];
}
