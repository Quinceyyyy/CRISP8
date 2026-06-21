
#include "cpu.h"
#include "opcodes.h"

#include <stdio.h>


void exec_ld_vx_dt(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_vx_dt] Status: Loading delay timer into Vx for opcode 0x%04X.\n", opcode);
}

void exec_ld_dt_vx(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_dt_vx] Status: Setting delay timer to Vx for opcode 0x%04X.\n", opcode);
}

void exec_ld_st_vx(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_st_vx] Status: Setting sound timer to Vx for opcode 0x%04X.\n", opcode);
}
