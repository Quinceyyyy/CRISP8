
#include "cpu.h"
#include "opcodes.h"

#include <stdio.h>


void exec_ld_vx_k(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_vx_k] Status: Waiting for key press for opcode 0x%04X.\n", opcode);
}
