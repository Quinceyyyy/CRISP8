
#include "cpu.h"

#include <stdint.h>
#include <stdio.h>


uint16_t fetch_opcode(Cpu *cpu)
{
    if (cpu->pc + 1 >= RAM_MEMORY) {
        fprintf(stderr, "[fetch_opcode] Error: pc is out of bounds ! Halting the CPU\n");
        cpu->halted = true;
        return 0;
    }

    uint8_t hbyte = cpu->memory[cpu->pc];
    uint8_t lbyte = cpu->memory[cpu->pc + 1];
    uint16_t opcode = (hbyte << 8) | lbyte;

    return opcode;
}
