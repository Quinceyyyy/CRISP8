
#include "cpu.h"
#include "opcodes.h"

#include <stdio.h>

void exec_ld_reg(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_reg] Status: Loading value from Vy to Vx for opcode 0x%04X.\n", opcode);
}
