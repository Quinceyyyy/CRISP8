
#include "cpu.h"

#include <stdint.h>
#include <stdio.h>


void exec_add(Cpu *cpu, uint16_t opcode)
{
    uint8_t register_X = (opcode >> 8) & 0xF; //X
    uint8_t value = opcode & 0xFF; //NN

    cpu->v_registers[register_X] += value;

    printf("[exec_add] Status: value %d has been added at register <0x%04x>.\n", value, register_X);
}
