
#include "cpu.h"
#include "utils.h"
#include "window.h"

#include "raylib.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


void init_cpu(Cpu *cpu)
{
    memset(cpu->memory, 0, RAM_MEMORY);
    cpu->pc = ROM_START_ADDRESS;
    cpu->halted = false;
}

int run_cpu(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Needs 2 args, bin + ROM.\n");
        return -1;
    }

    Cpu cpu = {0};
    init_cpu(&cpu);
    if (read_rom(&cpu, argv[1]) != 0) { return -1; }

    uint16_t opcode = 0;

    // init_window();

    for (;;) {
        opcode = fetch_opcode(&cpu);
        if (cpu.halted || opcode == 0) {
            break;
        }
        cpu.pc += 2;
        decode_opcode(&cpu, opcode); 

    }

    return 0;
}
