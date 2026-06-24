
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>


void exec_ld_vx_dt(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    cpu->v_registers[reg_X] = cpu->delay_timer;
}

void exec_ld_dt_vx(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    cpu->delay_timer = cpu->v_registers[reg_X];
}

void exec_ld_st_vx(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    cpu->sound_timer = cpu->v_registers[reg_X];
}
