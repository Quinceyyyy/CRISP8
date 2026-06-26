
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>


void exec_add_i(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    cpu->index_register += cpu->v_registers[reg_X];
}
