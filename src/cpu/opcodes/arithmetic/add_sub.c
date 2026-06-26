
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>


void exec_add_reg(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    uint16_t res = (uint16_t)cpu->v_registers[reg_X] + (uint16_t)cpu->v_registers[reg_Y];
    if (res > 0xFF) {
        cpu->v_registers[VF] = 1;
    } else {
        cpu->v_registers[VF] = 0;
    }

    cpu->v_registers[reg_X] = (uint8_t)res;
}


void exec_sub(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    if (cpu->v_registers[reg_X] >= cpu->v_registers[reg_Y]) {
        cpu->v_registers[VF] = 1;
    } else {
        cpu->v_registers[VF] = 0;
    }

    cpu->v_registers[reg_X] -= cpu->v_registers[reg_Y];
}


void exec_subn(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    if (cpu->v_registers[reg_Y] >= cpu->v_registers[reg_X]) {
        cpu->v_registers[VF] = 1;
    } else {
        cpu->v_registers[VF] = 0;
    }

    cpu->v_registers[reg_X] = cpu->v_registers[reg_Y] - cpu->v_registers[reg_X];
}
