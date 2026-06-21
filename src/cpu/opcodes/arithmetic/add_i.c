
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>

void exec_add_i(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_add_i] Status: Adding Vx to I for opcode 0x%04X.\n", opcode);
}
