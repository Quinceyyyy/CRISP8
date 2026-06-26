
#include "cpu.h"

#include <string.h>


void exec_clear_screen(Cpu *cpu)
{
    memset(cpu->display_buffer, 0, sizeof(cpu->display_buffer));
}
