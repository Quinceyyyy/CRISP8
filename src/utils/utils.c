
#include "cpu.h"
#include "utils.h"

#include <stdio.h>

void print_memory(Cpu *cpu, long fsize)
{ 
    for (int i = ROM_START_ADDRESS; i < ROM_START_ADDRESS + fsize; i++) {
        printf("0x%02X ", (unsigned int)cpu->memory[i]);
        if ((i - ROM_START_ADDRESS + 1) % 8 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

