
#include "cpu.h"
#include "opcodes.h"

#include <stdio.h>


void exec_ld_f_vx(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_f_vx] Status: Loading address of sprite for digit Vx into I for opcode 0x%04X.\n", opcode);
}

void exec_ld_i_vx(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_i_vx] Status: Storing V0..Vx into memory starting at I for opcode 0x%04X.\n", opcode);
}

void exec_ld_vx_i(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_vx_i] Status: Loading V0..Vx from memory starting at I for opcode 0x%04X.\n", opcode);
}
