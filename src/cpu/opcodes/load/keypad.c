
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>


void exec_ld_vx_k(Cpu *cpu, uint16_t opcode)
{
    uint8_t X = (opcode >> 8) & 0xF;
    bool key_pressed = false;

    for (int i = 0; i < MAX_INPUTS; i++) {
        if (cpu->input_pad[i] != 0) {
            cpu->v_registers[X] = i;
            key_pressed = true;
            break;
        }
    }

    if (!key_pressed) { cpu->pc -= 2; }
}
