
#include "cpu.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROM_SIZE (RAM_MEMORY - ROM_START_ADDRESS)


static bool check_file_size(long fsize)
{
    if (fsize > MAX_ROM_SIZE) { return false; }

    return true;
}

int read_rom(Cpu *cpu, char *rom_file)
{
    FILE *file = NULL;
    int ret = -1;

    if (rom_file == NULL) { return -1; }

    file = fopen(rom_file, "rb");
    if (file == NULL) {
        fprintf(stderr, "[read_rom] Error: fopen failed to read data from file <%s>.\n", rom_file);
        goto cleanup;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "[read_rom] Error: fseek failed to read the full file.\n");
        goto cleanup;
    }

    long fsize = ftell(file);
    if (fsize == -1L) {
        fprintf(stderr, "[read_rom] Error: ftell failed.\n");
        goto cleanup;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "[read_rom] Error: fseek failed to rewind the file.\n");
        goto cleanup;
    }

    if (!check_file_size(fsize)) {
        fprintf(stderr, "[read_rom] Error: Size of the ROM is to big.\n");
        goto cleanup;
    }

    size_t fres = fread(cpu->memory + ROM_START_ADDRESS, 1, (size_t)fsize, file);
    if (fres != (size_t)fsize) {
        fprintf(stderr, "[read_rom] Error: fread failed to read the file contents.\n");
        goto cleanup;
    }

    print_memory(cpu, fsize);
    ret = 0;

cleanup:
    if (file != NULL) { fclose(file); }
    return ret;
}
