
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>

void exec_skp(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_skp] Status: Checking if key is pressed for opcode 0x%04X.\n", opcode);
}

void exec_sknp(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_sknp] Status: Checking if key is NOT pressed for opcode 0x%04X.\n", opcode);
}
