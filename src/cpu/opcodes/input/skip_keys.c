
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>


void exec_skp(Cpu *cpu, uint16_t opcode)
{
    uint8_t VX = (opcode >> 8) & 0xF;
    uint8_t key = cpu->v_registers[VX] & 0xF;

    if (cpu->input_pad[key] != 0) {
        cpu->pc += 2;
    }
}

void exec_sknp(Cpu *cpu, uint16_t opcode)
{
    uint8_t VX = (opcode >> 8) & 0xF;
    uint8_t key = cpu->v_registers[VX] & 0xF;

    if (cpu->input_pad[key] == 0) {
        cpu->pc += 2;
    }
}
