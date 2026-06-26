
#include "cpu.h"


void exec_return(Cpu *cpu)
{
    cpu->sp -= 1;
    cpu->pc = cpu->stack[cpu->sp];
}
