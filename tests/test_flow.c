
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
/*  1NNN  exec_jump                                                     */
/* ------------------------------------------------------------------ */
static void test_exec_jump(void)
{
    Cpu cpu;

    /* Jumps to the embedded address */
    cpu = make_cpu();
    cpu.pc = 0x200;
    exec_jump(&cpu, opcode(0x13, 0x00));   /* JP 0x300 */
    ASSERT("JUMP: pc set to 0x300", cpu.pc == 0x300);

    /* Mask works — only lower 12 bits used */
    cpu = make_cpu();
    exec_jump(&cpu, opcode(0x1F, 0xFF));   /* JP 0xFFF */
    ASSERT("JUMP: pc set to 0xFFF", cpu.pc == 0xFFF);

    /* Jump to start of ROM */
    cpu = make_cpu();
    cpu.pc = 0x400;
    exec_jump(&cpu, opcode(0x12, 0x00));   /* JP 0x200 */
    ASSERT("JUMP: pc set to 0x200", cpu.pc == 0x200);
}

/* ------------------------------------------------------------------ */
/*  BNNN  exec_jump_offset  (pc = NNN + V0)                            */
/* ------------------------------------------------------------------ */
static void test_exec_jump_offset(void)
{
    Cpu cpu;

    /* V0 = 0: behaves like a plain jump */
    cpu = make_cpu();
    cpu.v_registers[V0] = 0;
    exec_jump_offset(&cpu, opcode(0xB3, 0x00));   /* JP V0, 0x300 */
    ASSERT("JUMP_OFFSET V0=0: pc == 0x300", cpu.pc == 0x300);

    /* V0 = 0x10: adds offset */
    cpu = make_cpu();
    cpu.v_registers[V0] = 0x10;
    exec_jump_offset(&cpu, opcode(0xB3, 0x00));   /* JP V0, 0x300 */
    ASSERT("JUMP_OFFSET V0=0x10: pc == 0x310", cpu.pc == 0x310);

    /* V0 = 0xFF: maximum V0 offset */
    cpu = make_cpu();
    cpu.v_registers[V0] = 0xFF;
    exec_jump_offset(&cpu, opcode(0xB2, 0x00));   /* JP V0, 0x200 */
    ASSERT("JUMP_OFFSET V0=0xFF: pc == 0x2FF", cpu.pc == 0x2FF);
}

/* ------------------------------------------------------------------ */
/*  2NNN  exec_call  +  00EE  exec_return                              */
/* ------------------------------------------------------------------ */
static void test_exec_call(void)
{
    Cpu cpu;

    /* PC is saved onto the stack, sp incremented, pc jumps */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.sp = 0;
    exec_call(&cpu, opcode(0x23, 0x00));   /* CALL 0x300 */
    ASSERT("CALL: return addr on stack",  cpu.stack[0] == 0x200);
    ASSERT("CALL: sp incremented to 1",  cpu.sp == 1);
    ASSERT("CALL: pc set to 0x300",      cpu.pc == 0x300);

    /* Nested call: second call stacks on top */
    exec_call(&cpu, opcode(0x24, 0x00));   /* CALL 0x400 */
    ASSERT("CALL nested: sp == 2",        cpu.sp == 2);
    ASSERT("CALL nested: stack[1] saved", cpu.stack[1] == 0x300);
    ASSERT("CALL nested: pc == 0x400",    cpu.pc == 0x400);
}

static void test_exec_return(void)
{
    Cpu cpu;

    /* Single return */
    cpu = make_cpu();
    cpu.stack[0] = 0x200;
    cpu.sp = 1;
    cpu.pc = 0x300;
    exec_return(&cpu);
    ASSERT("RETURN: sp decremented to 0", cpu.sp == 0);
    ASSERT("RETURN: pc restored to 0x200", cpu.pc == 0x200);

    /* Call then return restores exactly */
    cpu = make_cpu();
    cpu.pc = 0x200;
    exec_call(&cpu, opcode(0x23, 0x00));   /* CALL 0x300 */
    exec_return(&cpu);
    ASSERT("CALL+RETURN: pc back to 0x200", cpu.pc == 0x200);
    ASSERT("CALL+RETURN: sp back to 0",     cpu.sp == 0);
}

/* ------------------------------------------------------------------ */
/*  3XNN  exec_se_byte  (skip if VX == NN)                             */
/* ------------------------------------------------------------------ */
static void test_exec_se_byte(void)
{
    Cpu cpu;

    /* Equal: skip (pc += 2) */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x1] = 0x42;
    exec_se_byte(&cpu, opcode(0x31, 0x42));
    ASSERT("SE_BYTE equal: pc skips to 0x202", cpu.pc == 0x202);

    /* Not equal: no skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x1] = 0x10;
    exec_se_byte(&cpu, opcode(0x31, 0x42));
    ASSERT("SE_BYTE not equal: pc unchanged", cpu.pc == 0x200);
}

/* ------------------------------------------------------------------ */
/*  4XNN  exec_sne_byte  (skip if VX != NN)                            */
/* ------------------------------------------------------------------ */
static void test_exec_sne_byte(void)
{
    Cpu cpu;

    /* Not equal: skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x2] = 0x10;
    exec_sne_byte(&cpu, opcode(0x42, 0x42));
    ASSERT("SNE_BYTE not equal: pc skips to 0x202", cpu.pc == 0x202);

    /* Equal: no skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x2] = 0x42;
    exec_sne_byte(&cpu, opcode(0x42, 0x42));
    ASSERT("SNE_BYTE equal: pc unchanged", cpu.pc == 0x200);
}

/* ------------------------------------------------------------------ */
/*  5XY0  exec_se_reg  (skip if VX == VY)                              */
/* ------------------------------------------------------------------ */
static void test_exec_se_reg(void)
{
    Cpu cpu;

    /* Equal: skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x1] = 0x55;
    cpu.v_registers[0x2] = 0x55;
    exec_se_reg(&cpu, opcode(0x51, 0x20));
    ASSERT("SE_REG equal: pc skips to 0x202", cpu.pc == 0x202);

    /* Not equal: no skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x1] = 0x55;
    cpu.v_registers[0x2] = 0x56;
    exec_se_reg(&cpu, opcode(0x51, 0x20));
    ASSERT("SE_REG not equal: pc unchanged", cpu.pc == 0x200);

    /* Same register always equal */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x3] = 0xAB;
    exec_se_reg(&cpu, opcode(0x53, 0x30));
    ASSERT("SE_REG same reg: always skips", cpu.pc == 0x202);
}

/* ------------------------------------------------------------------ */
/*  9XY0  exec_sne_reg  (skip if VX != VY)                             */
/* ------------------------------------------------------------------ */
static void test_exec_sne_reg(void)
{
    Cpu cpu;

    /* Not equal: skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x1] = 0x10;
    cpu.v_registers[0x2] = 0x20;
    exec_sne_reg(&cpu, opcode(0x91, 0x20));
    ASSERT("SNE_REG not equal: pc skips to 0x202", cpu.pc == 0x202);

    /* Equal: no skip */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x1] = 0x10;
    cpu.v_registers[0x2] = 0x10;
    exec_sne_reg(&cpu, opcode(0x91, 0x20));
    ASSERT("SNE_REG equal: pc unchanged", cpu.pc == 0x200);

    /* Same register: always equal, never skips */
    cpu = make_cpu();
    cpu.pc = 0x200;
    cpu.v_registers[0x3] = 0xAB;
    exec_sne_reg(&cpu, opcode(0x93, 0x30));
    ASSERT("SNE_REG same reg: never skips", cpu.pc == 0x200);
}

/* ------------------------------------------------------------------ */
/*  Suite entry (called from test_main.c)                              */
/* ------------------------------------------------------------------ */
void run_flow_tests(void)
{
    printf("\n=== Flow/Jump Opcodes ===\n");

    RUN_SUITE(test_exec_jump);
    RUN_SUITE(test_exec_jump_offset);
    RUN_SUITE(test_exec_call);
    RUN_SUITE(test_exec_return);
    RUN_SUITE(test_exec_se_byte);
    RUN_SUITE(test_exec_sne_byte);
    RUN_SUITE(test_exec_se_reg);
    RUN_SUITE(test_exec_sne_reg);
}
