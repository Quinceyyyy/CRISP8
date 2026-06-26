
#include "cpu.h"

#include <stdint.h>


void exec_call(Cpu *cpu, uint16_t opcode)
{
    uint16_t dest_addr = opcode & 0x0FFF;

    cpu->stack[cpu->sp] = cpu->pc;
    cpu->sp += 1;
    cpu->pc = dest_addr;
}
