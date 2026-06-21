
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>

void exec_draw(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_draw] Status: Drawing sprite for opcode 0x%04X.\n", opcode);
}
