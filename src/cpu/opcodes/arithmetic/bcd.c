
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>

void exec_ld_b_vx(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_b_vx] Status: Loading BCD representation of Vx for opcode 0x%04X.\n", opcode);

}
