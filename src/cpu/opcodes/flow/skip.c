
#include "cpu.h"

#include <stdint.h>
#include <stdio.h>


void exec_se_byte(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_se_byte] Status: Checking if Vx == NN for opcode 0x%04X.\n", opcode);
}

void exec_sne_byte(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_sne_byte] Status: Checking if Vx != NN for opcode 0x%04X.\n", opcode);

}

void exec_se_reg(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_se_reg] Status: Checking if Vx == Vy for opcode 0x%04X.\n", opcode);
}

void exec_sne_reg(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_sne_reg] Status: Checking if Vx != Vy for opcode 0x%04X.\n", opcode);

}
