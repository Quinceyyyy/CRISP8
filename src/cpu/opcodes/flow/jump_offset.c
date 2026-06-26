
#include "cpu.h"

#include <stdint.h>


void exec_jump_offset(Cpu *cpu, uint16_t opcode)
{
    uint16_t dest_addr = opcode & 0x0FFF;
    cpu->pc = dest_addr + cpu->v_registers[V0];
}
