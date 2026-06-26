
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>

// Original CHIP8 did VY shift instead.

void exec_shr(Cpu *cpu, uint16_t opcode)
{
    uint8_t VX = (opcode >> 8) & 0xF;
    uint8_t LSB = cpu->v_registers[VX] & 1;
    cpu->v_registers[VF] = LSB;
    cpu->v_registers[VX] >>= 1;
}


void exec_shl(Cpu *cpu, uint16_t opcode)
{
    uint8_t VX = (opcode >> 8) & 0xF;
    uint8_t MSB = (cpu->v_registers[VX] >> 7) & 1;
    cpu->v_registers[VF] = MSB;
    cpu->v_registers[VX] <<= 1;
}
