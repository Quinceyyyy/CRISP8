
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>


void exec_ld_f_vx(Cpu *cpu, uint16_t opcode)
{
    printf("[exec_ld_f_vx] Status: Loading address of sprite for digit Vx into I for opcode 0x%04X.\n", opcode);
}

void exec_ld_i_vx(Cpu *cpu, uint16_t opcode)
{
    uint8_t X = (opcode >> 8) & 0xF;

    for (uint16_t i = 0; i <= X; i++) {
        if (cpu->index_register + i < RAM_MEMORY) {
            cpu->memory[cpu->index_register + i] = cpu->v_registers[i];
        }
    }
}

void exec_ld_vx_i(Cpu *cpu, uint16_t opcode)
{
    uint8_t X = (opcode >> 8) & 0xF;

    for (uint16_t i = 0; i <= X; i++) {
        if (cpu->index_register + i < RAM_MEMORY) {
            cpu->v_registers[i] = cpu->memory[cpu->index_register + i];
        }
    }
}
