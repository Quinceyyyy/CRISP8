
#include "cpu.h"
#include "opcodes.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


void exec_ld_b_vx(Cpu *cpu, uint16_t opcode)
{
    uint8_t VX = (opcode >> 8) & 0xF;
    uint8_t BCD = cpu->v_registers[VX];

    if (cpu->index_register + 2 >= RAM_MEMORY) {
        fprintf(stderr, "[exec_ld_b_vx] Error: index_register is out of bounds ! Halting cpu..\n");
        cpu->halted = true;
        return;
    }

    cpu->memory[cpu->index_register] = (BCD / 100);
    cpu->memory[cpu->index_register + 1] = (BCD / 10) % 10;
    cpu->memory[cpu->index_register + 2] = (BCD % 10);
}
