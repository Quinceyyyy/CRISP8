
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdlib.h>


void exec_rand(Cpu *cpu, uint16_t opcode)
{
    uint8_t reg_X = (opcode >> 8) & 0xF;
    uint8_t NN = opcode & 0xFF; 

    cpu->v_registers[reg_X] = rand() & NN;
}
