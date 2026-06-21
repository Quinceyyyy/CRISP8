
#include "cpu.h"
#include "opcodes.h"

#include <stdlib.h>
#include <stdio.h>


void exec_rand(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_rand] Status: Generating random number for opcode 0x%04X.\n", opcode);
}
