
#include "cpu.h"
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
}

static void cpu_cycles(Cpu *cpu)
{
    for (int i = 0; i < CYCLES_PER_FRAME; i++) {
        uint16_t opcode = fetch_opcode(cpu);
        if (cpu->halted) {
            break;
        }

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

    // init_window();

    for (;;) {
        cpu_cycles(&cpu);
        if (cpu.halted) {
            break;
        }
        if (cpu.delay_timer > 0) {
            cpu.delay_timer--;
        }

        if (cpu.sound_timer > 0) {
            cpu.sound_timer--;
        }

    }

    return 0;
}
