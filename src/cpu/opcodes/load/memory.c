
#include "cpu.h"
#include "opcodes.h"
#include "font.h"

#include <stdint.h>


void exec_ld_f_vx(Cpu *cpu, uint16_t opcode)
{
    uint8_t VX = (opcode >> 8) & 0xF;
    uint8_t fval = (cpu->v_registers[VX] & 0xF);

    cpu->index_register = FONT_START_ADDR + (fval * 5);
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
