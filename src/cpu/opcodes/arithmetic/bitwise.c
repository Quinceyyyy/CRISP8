
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>

void exec_or(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_or] Status: Performing OR operation for opcode 0x%04X.\n", opcode);
}

void exec_and(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_and] Status: Performing AND operation for opcode 0x%04X.\n", opcode);
}

void exec_xor(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_xor] Status: Performing XOR operation for opcode 0x%04X.\n", opcode);
}
