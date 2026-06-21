
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>

void exec_add_reg(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_add_reg] Status: Performing addition for opcode 0x%04X.\n", opcode);
}


void exec_sub(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_sub] Status: Performing subtraction for opcode 0x%04X.\n", opcode);
}


void exec_subn(Cpu *cpu, uint16_t opcode)
{
 printf("[exec_subn] Status: Performing reverse subtraction for opcode 0x%04X.\n", opcode);
}
