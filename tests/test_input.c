
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Shared harness counters (defined in test_main.c)                   */
/* ------------------------------------------------------------------ */
extern int tests_run;
extern int tests_passed;
extern int tests_failed;

#define ASSERT(desc, cond)                                              \
    do {                                                                \
        tests_run++;                                                    \
        if (cond) {                                                     \
            printf("  [PASS] %s\n", desc);                             \
            tests_passed++;                                             \
        } else {                                                        \
            printf("  [FAIL] %s  (line %d)\n", desc, __LINE__);       \
            tests_failed++;                                             \
        }                                                               \
    } while (0)

#define RUN_SUITE(name)                                                 \
    do {                                                                \
        printf("\n== %s ==\n", #name);                                 \
        name();                                                         \
    } while (0)

static Cpu make_cpu(void)
{
    Cpu cpu;
    memset(&cpu, 0, sizeof(Cpu));
    return cpu;
}

static uint16_t opcode(uint8_t hi, uint8_t lo)
{
    return (uint16_t)((hi << 8) | lo);
}

/* ------------------------------------------------------------------ */
/*  EX9E  exec_skp   -- STUB                                           */
/*  EXA1  exec_sknp  -- STUB                                           */
/*  EX9E  exec_skp  (skip if key pressed)                              */
/* ------------------------------------------------------------------ */
static void test_exec_skp(void)
{
    Cpu cpu;

    /* Key is pressed: skips next instruction */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[V1] = 0xA;
    cpu.input_pad[0xA] = 1;
    exec_skp(&cpu, opcode(0xE1, 0x9E));
    ASSERT("SKP pressed: skips to 0x202", cpu.pc == 0x202);

    /* Key is NOT pressed: does not skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[V2] = 0xB;
    cpu.input_pad[0xB] = 0;
    exec_skp(&cpu, opcode(0xE2, 0x9E));
    ASSERT("SKP not pressed: pc unchanged", cpu.pc == 0x200);
}

/* ------------------------------------------------------------------ */
/*  EXA1  exec_sknp  (skip if key NOT pressed)                         */
/* ------------------------------------------------------------------ */
static void test_exec_sknp(void)
{
    Cpu cpu;

    /* Key is NOT pressed: skips next instruction */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[V3] = 0xC;
    cpu.input_pad[0xC] = 0;
    exec_sknp(&cpu, opcode(0xE3, 0xA1));
    ASSERT("SKNP not pressed: skips to 0x202", cpu.pc == 0x202);

    /* Key is pressed: does not skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[V4] = 0xD;
    cpu.input_pad[0xD] = 1;
    exec_sknp(&cpu, opcode(0xE4, 0xA1));
    ASSERT("SKNP pressed: pc unchanged", cpu.pc == 0x200);
}

/* ------------------------------------------------------------------ */
/*  Suite entry (called from test_main.c)                              */
/* ------------------------------------------------------------------ */
void run_input_tests(void)
{
    printf("\n=== Input Opcodes ===\n");

    RUN_SUITE(test_exec_skp);
    RUN_SUITE(test_exec_sknp);
}
