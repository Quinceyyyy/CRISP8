
#include "cpu.h"
#include "opcodes.h"

#include <stdio.h>

void exec_load_i(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_load_i] Status: Loading address NNN into I for opcode 0x%04X.\n", opcode);
}
