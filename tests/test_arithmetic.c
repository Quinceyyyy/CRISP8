
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

/* Produce a fresh zeroed CPU for each test */
static Cpu make_cpu(void)
{
    Cpu cpu;
    memset(&cpu, 0, sizeof(Cpu));
    return cpu;
}

/* Build opcode word from nibbles: 0xABCD */
static uint16_t opcode(uint8_t hi_byte, uint8_t lo_byte)
{
    return (uint16_t)((hi_byte << 8) | lo_byte);
}

/* ------------------------------------------------------------------ */
/*  7XNN  exec_add  (no carry flag)                                    */
/* ------------------------------------------------------------------ */
static void test_exec_add(void)
{
    Cpu cpu;

    /* Basic addition */
    cpu = make_cpu();
    cpu.v_registers[0x2] = 10;
    exec_add(&cpu, opcode(0x72, 0x05));   /* ADD V2, 5 */
    ASSERT("ADD V2,5 -> 15", cpu.v_registers[0x2] == 15);

    /* Wraps on overflow -- no flag is set */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0xFF;
    exec_add(&cpu, opcode(0x71, 0x01));   /* ADD V1, 1 */
    ASSERT("ADD 0xFF+1 wraps to 0x00", cpu.v_registers[0x1] == 0x00);
    ASSERT("ADD overflow does NOT set VF", cpu.v_registers[VF] == 0);
}

/* ------------------------------------------------------------------ */
/*  8XY4  exec_add_reg  (sets VF carry)                               */
/* ------------------------------------------------------------------ */
static void test_exec_add_reg(void)
{
    Cpu cpu;

    /* No carry */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 10;
    cpu.v_registers[0x2] = 20;
    exec_add_reg(&cpu, opcode(0x81, 0x24));  /* ADD V1, V2 */
    ASSERT("ADD_REG no carry: result 30", cpu.v_registers[0x1] == 30);
    ASSERT("ADD_REG no carry: VF == 0",  cpu.v_registers[VF] == 0);

    /* Carry */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0xFF;
    cpu.v_registers[0x2] = 0x02;
    exec_add_reg(&cpu, opcode(0x81, 0x24));
    ASSERT("ADD_REG carry: VF == 1",          cpu.v_registers[VF] == 1);
    ASSERT("ADD_REG carry: result wraps 0x01", cpu.v_registers[0x1] == 0x01);
}

/* ------------------------------------------------------------------ */
/*  8XY5  exec_sub  (VF = NOT borrow: 1 if VX >= VY)                 */
/* ------------------------------------------------------------------ */
static void test_exec_sub(void)
{
    Cpu cpu;

    /* VX > VY, no borrow */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 10;
    cpu.v_registers[0x2] = 3;
    exec_sub(&cpu, opcode(0x81, 0x25));   /* SUB V1, V2 */
    ASSERT("SUB no borrow: result 7",  cpu.v_registers[0x1] == 7);
    ASSERT("SUB no borrow: VF == 1",   cpu.v_registers[VF] == 1);

    /* VX == VY */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 5;
    cpu.v_registers[0x2] = 5;
    exec_sub(&cpu, opcode(0x81, 0x25));
    ASSERT("SUB equal: result 0",     cpu.v_registers[0x1] == 0);
    ASSERT("SUB equal: VF == 1",      cpu.v_registers[VF] == 1);

    /* VX < VY, borrow */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 3;
    cpu.v_registers[0x2] = 10;
    exec_sub(&cpu, opcode(0x81, 0x25));
    ASSERT("SUB borrow: VF == 0",      cpu.v_registers[VF] == 0);
    ASSERT("SUB borrow: result wraps", cpu.v_registers[0x1] == (uint8_t)(3 - 10));
}

/* ------------------------------------------------------------------ */
/*  8XY7  exec_subn  (VF = NOT borrow: 1 if VY >= VX)                */
/* ------------------------------------------------------------------ */
static void test_exec_subn(void)
{
    Cpu cpu;

    /* VY > VX, no borrow */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 3;
    cpu.v_registers[0x2] = 10;
    exec_subn(&cpu, opcode(0x81, 0x27));  /* SUBN V1, V2  =>  V1 = V2 - V1 */
    ASSERT("SUBN no borrow: result 7", cpu.v_registers[0x1] == 7);
    ASSERT("SUBN no borrow: VF == 1",  cpu.v_registers[VF] == 1);

    /* VY == VX */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 5;
    cpu.v_registers[0x2] = 5;
    exec_subn(&cpu, opcode(0x81, 0x27));
    ASSERT("SUBN equal: result 0",    cpu.v_registers[0x1] == 0);
    ASSERT("SUBN equal: VF == 1",     cpu.v_registers[VF] == 1);

    /* VY < VX, borrow */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 10;
    cpu.v_registers[0x2] = 3;
    exec_subn(&cpu, opcode(0x81, 0x27));
    ASSERT("SUBN borrow: VF == 0", cpu.v_registers[VF] == 0);
}

/* ------------------------------------------------------------------ */
/*  8XY1/2/3  exec_or / exec_and / exec_xor                           */
/* ------------------------------------------------------------------ */
static void test_exec_bitwise(void)
{
    Cpu cpu;

    /* OR */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0b10101010;
    cpu.v_registers[0x2] = 0b01010101;
    exec_or(&cpu, opcode(0x81, 0x21));
    ASSERT("OR: 0xAA | 0x55 == 0xFF", cpu.v_registers[0x1] == 0xFF);

    /* AND */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0b11110000;
    cpu.v_registers[0x2] = 0b10101010;
    exec_and(&cpu, opcode(0x81, 0x22));
    ASSERT("AND: 0xF0 & 0xAA == 0xA0", cpu.v_registers[0x1] == 0xA0);

    /* XOR */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0xFF;
    cpu.v_registers[0x2] = 0xFF;
    exec_xor(&cpu, opcode(0x81, 0x23));
    ASSERT("XOR: 0xFF ^ 0xFF == 0x00", cpu.v_registers[0x1] == 0x00);

    cpu = make_cpu();
    cpu.v_registers[0x1] = 0b10101010;
    cpu.v_registers[0x2] = 0b01010101;
    exec_xor(&cpu, opcode(0x81, 0x23));
    ASSERT("XOR: 0xAA ^ 0x55 == 0xFF", cpu.v_registers[0x1] == 0xFF);
}

/* ------------------------------------------------------------------ */
/*  8XY6  exec_shr  (modern: shift VX in place, VF = old LSB)         */
/* ------------------------------------------------------------------ */
static void test_exec_shr(void)
{
    Cpu cpu;

    /* LSB was 1 */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0b00000101;   /* 5 */
    exec_shr(&cpu, opcode(0x81, 0x06));
    ASSERT("SHR LSB=1: result is 2",  cpu.v_registers[0x1] == 2);
    ASSERT("SHR LSB=1: VF == 1",      cpu.v_registers[VF] == 1);

    /* LSB was 0 */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0b00001000;   /* 8 */
    exec_shr(&cpu, opcode(0x81, 0x06));
    ASSERT("SHR LSB=0: result is 4",  cpu.v_registers[0x1] == 4);
    ASSERT("SHR LSB=0: VF == 0",      cpu.v_registers[VF] == 0);
}

/* ------------------------------------------------------------------ */
/*  8XYE  exec_shl  (modern: shift VX in place, VF = old MSB)         */
/* ------------------------------------------------------------------ */
static void test_exec_shl(void)
{
    Cpu cpu;

    /* MSB was 1 */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0b10000010;   /* 0x82 */
    exec_shl(&cpu, opcode(0x81, 0x0E));
    ASSERT("SHL MSB=1: result is 0x04", cpu.v_registers[0x1] == 0x04);
    ASSERT("SHL MSB=1: VF == 1",        cpu.v_registers[VF] == 1);

    /* MSB was 0 */
    cpu = make_cpu();
    cpu.v_registers[0x1] = 0b00000010;   /* 2 */
    exec_shl(&cpu, opcode(0x81, 0x0E));
    ASSERT("SHL MSB=0: result is 4",    cpu.v_registers[0x1] == 4);
    ASSERT("SHL MSB=0: VF == 0",        cpu.v_registers[VF] == 0);
}

/* ------------------------------------------------------------------ */
/*  FX1E  exec_add_i  (I += VX)                                       */
/* ------------------------------------------------------------------ */
static void test_exec_add_i(void)
{
    Cpu cpu;

    cpu = make_cpu();
    cpu.index_register = 0x300;
    cpu.v_registers[0x3] = 0x10;
    exec_add_i(&cpu, opcode(0xF3, 0x1E));
    ASSERT("ADD_I: I = 0x300 + 0x10 == 0x310", cpu.index_register == 0x310);

    /* Zero register adds nothing */
    cpu = make_cpu();
    cpu.index_register = 0x200;
    cpu.v_registers[0x0] = 0;
    exec_add_i(&cpu, opcode(0xF0, 0x1E));
    ASSERT("ADD_I: I unchanged when VX=0", cpu.index_register == 0x200);
}

/* ------------------------------------------------------------------ */
/*  FX33  exec_ld_b_vx  (BCD)                                         */
/* ------------------------------------------------------------------ */
static void test_exec_ld_b_vx(void)
{
    Cpu cpu;

    /* 234 -> 2, 3, 4 */
    cpu = make_cpu();
    cpu.index_register = 0x300;
    cpu.v_registers[0x1] = 234;
    exec_ld_b_vx(&cpu, opcode(0xF1, 0x33));
    ASSERT("BCD 234: hundreds == 2", cpu.memory[0x300] == 2);
    ASSERT("BCD 234: tens     == 3", cpu.memory[0x301] == 3);
    ASSERT("BCD 234: ones     == 4", cpu.memory[0x302] == 4);

    /* 0 -> 0, 0, 0 */
    cpu = make_cpu();
    cpu.index_register = 0x300;
    cpu.v_registers[0x2] = 0;
    exec_ld_b_vx(&cpu, opcode(0xF2, 0x33));
    ASSERT("BCD 0: all digits 0", cpu.memory[0x300] == 0 &&
                                   cpu.memory[0x301] == 0 &&
                                   cpu.memory[0x302] == 0);

    /* 255 -> 2, 5, 5 */
    cpu = make_cpu();
    cpu.index_register = 0x300;
    cpu.v_registers[0x3] = 255;
    exec_ld_b_vx(&cpu, opcode(0xF3, 0x33));
    ASSERT("BCD 255: hundreds == 2", cpu.memory[0x300] == 2);
    ASSERT("BCD 255: tens     == 5", cpu.memory[0x301] == 5);
    ASSERT("BCD 255: ones     == 5", cpu.memory[0x302] == 5);

    /* Out-of-bounds guard: halts cpu, does not write */
    cpu = make_cpu();
    cpu.index_register = RAM_MEMORY - 1;  /* I+2 would overflow */
    cpu.v_registers[0x1] = 42;
    exec_ld_b_vx(&cpu, opcode(0xF1, 0x33));
    ASSERT("BCD OOB: cpu halted", cpu.halted == true);
}

/* ------------------------------------------------------------------ */
/*  Suite entry (called from test_main.c)                              */
/* ------------------------------------------------------------------ */
void run_arithmetic_tests(void)
{
    printf("\n=== Arithmetic Opcodes ===\n");

    RUN_SUITE(test_exec_add);
    RUN_SUITE(test_exec_add_reg);
    RUN_SUITE(test_exec_sub);
    RUN_SUITE(test_exec_subn);
    RUN_SUITE(test_exec_bitwise);
    RUN_SUITE(test_exec_shr);
    RUN_SUITE(test_exec_shl);
    RUN_SUITE(test_exec_add_i);
    RUN_SUITE(test_exec_ld_b_vx);
}
