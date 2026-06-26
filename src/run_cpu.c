
#include "cpu.h"
#include "disasm.h"
#include "font.h"
#include "utils.h"
#include "window.h"

#include "raylib.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


void init_cpu(Cpu *cpu)
{
    memset(cpu->memory, 0, RAM_MEMORY);
    cpu->pc = ROM_START_ADDRESS;
    cpu->halted = false;

    for (int i = 0; i < FONTSET_SIZE; i++) {
        cpu->memory[FONT_START_ADDR + i] = chip8_fontset[i];
    }
}

static void cpu_cycles(Cpu *cpu)
{
    for (int i = 0; i < CYCLES_PER_FRAME; i++) {
        uint16_t opcode = fetch_opcode(cpu);
        if (cpu->halted) {
            break;
        }

#ifdef DEBUG
        trace_opcode(cpu, opcode);
#endif
        cpu->pc += 2;
        decode_opcode(cpu, opcode);
    }
}

int run_cpu(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Needs 2 args, bin + ROM.\n");
        return -1;
    }

    Cpu cpu = {0};
    init_cpu(&cpu);
    srand(time(NULL));
    if (read_rom(&cpu, argv[1]) != 0) { return -1; }

    init_window();

    while (!WindowShouldClose() && !cpu.halted) {
        update_user_input(&cpu);
        cpu_cycles(&cpu);

        if (cpu.delay_timer > 0) {
            cpu.delay_timer--;
        }
        if (cpu.sound_timer > 0) {
            cpu.sound_timer--;
        }
        draw_content(&cpu);
    }

    CloseWindow();

    return 0;
}
