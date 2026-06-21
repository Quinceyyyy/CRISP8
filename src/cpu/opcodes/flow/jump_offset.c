
#include "cpu.h"

#include <stdint.h>
#include <stdio.h>


void exec_jump_offset(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_jump_offset] Status: Jumping to offset address 0x%04X from pc 0x%04X.\n", opcode, cpu->pc);
}
