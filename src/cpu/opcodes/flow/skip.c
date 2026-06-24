
#include "cpu.h"

#include <stdint.h>


void exec_se_byte(Cpu *cpu, uint16_t opcode)
{
    uint8_t register_X = (opcode >> 8) & 0xF; //X
    uint8_t value = opcode & 0xFF; //NN

    if (cpu->v_registers[register_X] == value) {
        cpu->pc += 2;
    }
}

void exec_sne_byte(Cpu *cpu, uint16_t opcode)
{
    uint8_t register_X = (opcode >> 8) & 0xF; //X
    uint8_t value = opcode & 0xFF; //NN

    if (cpu->v_registers[register_X] != value) {
        cpu->pc += 2;
    }
}

void exec_se_reg(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    if (cpu->v_registers[reg_X] == cpu->v_registers[reg_Y]) {
        cpu->pc += 2;
    }
}

void exec_sne_reg(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t reg_Y = (opcode >> 4) & 0xF;

    if (cpu->v_registers[reg_X] != cpu->v_registers[reg_Y]) {
        cpu->pc += 2;
    }
}
