
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>

void exec_shr(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_shr] Status: Shifting Vx right by 1 for opcode 0x%04X.\n", opcode);
}


void exec_shl(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_shl] Status: Shifting Vx left by 1 for opcode 0x%04X.\n", opcode);
}
