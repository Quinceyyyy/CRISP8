
#include "cpu.h"
#include "opcodes.h"
#include "window.h"

#include <stdint.h>


void exec_draw(Cpu *cpu, uint16_t opcode)
{
    uint8_t VX = (opcode >> 8) & 0xF;
    uint8_t VY = (opcode >> 4) & 0xF;
    uint8_t height = opcode & 0xF; 
    
    uint8_t start_x = cpu->v_registers[VX] % SCREEN_WIDTH ;
    uint8_t start_y = cpu->v_registers[VY] % SCREEN_HEIGHT;

    cpu->v_registers[VF] = 0; // NO COLLISIONS YET 

    for (uint8_t row = 0; row < height; row++) {
        uint8_t y = start_y + row;
        if (y >= 32) {
            continue; // Clip bottom edge
        }
        
        uint8_t sprite_byte = cpu->memory[cpu->index_register + row];

        for (uint8_t col = 0; col < 8; col++) {
            uint8_t x = start_x + col;
            if (x >= 64) {
                continue; // Clip right edge
            }

            // Check if the current pixel in the sprite row is 1
            if (sprite_byte & (0x80 >> col)) {
                uint16_t idx = (y * 64) + x;

                if (cpu->display_buffer[idx] == 1) {
                    cpu->v_registers[VF] = 1;
                }

                cpu->display_buffer[idx] ^= 1;
            }
        }
    }
}
