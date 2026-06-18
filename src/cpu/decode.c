
#include "cpu.h"

#include <stdint.h>
#include <stdio.h>


int decode_opcode(Cpu *cpu, uint16_t opcode)
{
    printf("opcode at pc [%d]: 0x%04x\n", cpu->pc, opcode);
    return 0;
}
