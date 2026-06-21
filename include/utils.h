
#ifndef UTILS_H
    #define UTILS_H

    typedef struct cpu Cpu;

    int read_rom(Cpu *cpu, char *rom_file);

    void print_memory(Cpu *cpu, long fsize);

#endif
