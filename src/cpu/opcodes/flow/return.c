
#include "cpu.h"

#include <stdio.h>


void exec_return(Cpu *cpu)
{
    cpu->sp -= 1;
    cpu->pc = cpu->stack[cpu->sp];

    printf("[exec_return] Status: returning to address: %d.\n", cpu->pc);
}
