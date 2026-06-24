
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>


void exec_load_i(Cpu *cpu, uint16_t opcode)
{
    uint16_t dest_addr = opcode & 0x0FFF;
    cpu->index_register = dest_addr;
}
