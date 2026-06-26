
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

/* Build opcode word from high byte and low byte */
static uint16_t opcode(uint8_t hi, uint8_t lo)
{
    return (uint16_t)((hi << 8) | lo);
}

/* ------------------------------------------------------------------ */
/*  6XNN  exec_store  (VX = NN)                                        */
/* ------------------------------------------------------------------ */
static void test_exec_store(void)
{
    Cpu cpu;

    /* Store 0x42 into V0 */
    cpu = make_cpu();
    exec_store(&cpu, opcode(0x60, 0x42));
    ASSERT("STORE V0 = 0x42", cpu.v_registers[V0] == 0x42);

    /* Store 0x00 into V5 (zero stays zero) */
    cpu = make_cpu();
    cpu.v_registers[V5] = 0xFF;
    exec_store(&cpu, opcode(0x65, 0x00));
    ASSERT("STORE V5 = 0x00 overwrites existing value", cpu.v_registers[V5] == 0x00);

    /* Store 0xFF into VF */
    cpu = make_cpu();
    exec_store(&cpu, opcode(0x6F, 0xFF));
    ASSERT("STORE VF = 0xFF", cpu.v_registers[VF] == 0xFF);

    /* Store preserves unrelated registers */
    cpu = make_cpu();
    cpu.v_registers[V1] = 0xAB;
    exec_store(&cpu, opcode(0x63, 0x10));   /* STORE V3 = 0x10 */
    ASSERT("STORE V3: V1 unaffected", cpu.v_registers[V1] == 0xAB);
    ASSERT("STORE V3 = 0x10",         cpu.v_registers[V3] == 0x10);

    /* Store 0x01 into every register index via V2 */
    cpu = make_cpu();
    exec_store(&cpu, opcode(0x62, 0x01));
    ASSERT("STORE V2 = 0x01", cpu.v_registers[V2] == 0x01);

    /* Verify high nibble of opcode is masked correctly for VA */
    cpu = make_cpu();
    exec_store(&cpu, opcode(0x6A, 0xBB));
    ASSERT("STORE VA = 0xBB", cpu.v_registers[VA] == 0xBB);
}

/* ------------------------------------------------------------------ */
/*  8XY0  exec_ld_reg  (VX = VY)                                       */
/* ------------------------------------------------------------------ */
static void test_exec_ld_reg(void)
{
    Cpu cpu;

    /* Copy V2 into V1 */
    cpu = make_cpu();
    cpu.v_registers[V1] = 0x00;
    cpu.v_registers[V2] = 0x55;
    exec_ld_reg(&cpu, opcode(0x81, 0x20));   /* LD V1, V2 */
    ASSERT("LD_REG V1 = V2 (0x55)", cpu.v_registers[V1] == 0x55);
    ASSERT("LD_REG V2 unchanged",    cpu.v_registers[V2] == 0x55);

    /* Copy V0 (zero) into VB */
    cpu = make_cpu();
    cpu.v_registers[VB] = 0xAA;
    cpu.v_registers[V0] = 0x00;
    exec_ld_reg(&cpu, opcode(0x8B, 0x00));   /* LD VB, V0 */
    ASSERT("LD_REG VB = V0 (0x00) overwrites", cpu.v_registers[VB] == 0x00);

    /* Self-copy: VX = VX (idempotent) */
    cpu = make_cpu();
    cpu.v_registers[V3] = 0x7E;
    exec_ld_reg(&cpu, opcode(0x83, 0x30));   /* LD V3, V3 */
    ASSERT("LD_REG V3 = V3 (self-copy)", cpu.v_registers[V3] == 0x7E);

    /* Copy VF into V4 -- VF is a valid source */
    cpu = make_cpu();
    cpu.v_registers[VF] = 0x01;
    cpu.v_registers[V4] = 0x00;
    exec_ld_reg(&cpu, opcode(0x84, 0xF0));   /* LD V4, VF */
    ASSERT("LD_REG V4 = VF (0x01)", cpu.v_registers[V4] == 0x01);

    /* Copy 0xFF value between two non-zero registers */
    cpu = make_cpu();
    cpu.v_registers[V6] = 0xFF;
    cpu.v_registers[V7] = 0x00;
    exec_ld_reg(&cpu, opcode(0x87, 0x60));   /* LD V7, V6 */
    ASSERT("LD_REG V7 = V6 (0xFF)", cpu.v_registers[V7] == 0xFF);
}

/* ------------------------------------------------------------------ */
/*  ANNN  exec_load_i  (I = NNN)                                       */
/* ------------------------------------------------------------------ */
static void test_exec_load_i(void)
{
    Cpu cpu;

    /* Load 0x300 into I */
    cpu = make_cpu();
    exec_load_i(&cpu, opcode(0xA3, 0x00));
    ASSERT("LOAD_I: I = 0x300", cpu.index_register == 0x300);

    /* Load 0x000 into I (minimum) */
    cpu = make_cpu();
    cpu.index_register = 0x500;
    exec_load_i(&cpu, opcode(0xA0, 0x00));
    ASSERT("LOAD_I: I = 0x000", cpu.index_register == 0x000);

    /* Load 0xFFF into I (maximum 12-bit) */
    cpu = make_cpu();
    exec_load_i(&cpu, opcode(0xAF, 0xFF));
    ASSERT("LOAD_I: I = 0xFFF", cpu.index_register == 0xFFF);

    /* Load ROM start address */
    cpu = make_cpu();
    exec_load_i(&cpu, opcode(0xA2, 0x00));
    ASSERT("LOAD_I: I = 0x200 (ROM start)", cpu.index_register == 0x200);

    /* Upper nibble of opcode must be masked off -- only low 12 bits */
    cpu = make_cpu();
    exec_load_i(&cpu, opcode(0xA5, 0xAB));
    ASSERT("LOAD_I: I = 0x5AB (low 12 bits only)", cpu.index_register == 0x5AB);

    /* Does not affect v_registers */
    cpu = make_cpu();
    cpu.v_registers[V1] = 0x77;
    exec_load_i(&cpu, opcode(0xA4, 0x20));
    ASSERT("LOAD_I: V1 unaffected", cpu.v_registers[V1] == 0x77);
}

/* ------------------------------------------------------------------ */
/*  FX07  exec_ld_vx_dt  (VX = delay_timer)                           */
/*  FX15  exec_ld_dt_vx  (delay_timer = VX)                           */
/*  FX18  exec_ld_st_vx  (sound_timer = VX)                           */
/* ------------------------------------------------------------------ */
static void test_exec_timers(void)
{
    Cpu cpu;

    /* FX07: read delay_timer into VX */
    cpu = make_cpu();
    cpu.delay_timer = 0x3C;
    exec_ld_vx_dt(&cpu, opcode(0xF2, 0x07));   /* LD V2, DT */
    ASSERT("LD_VX_DT: V2 = delay_timer (0x3C)", cpu.v_registers[V2] == 0x3C);

    /* FX07: delay_timer == 0 -> VX = 0 */
    cpu = make_cpu();
    cpu.v_registers[V1] = 0xFF;
    cpu.delay_timer = 0x00;
    exec_ld_vx_dt(&cpu, opcode(0xF1, 0x07));
    ASSERT("LD_VX_DT: V1 = 0 when delay_timer == 0", cpu.v_registers[V1] == 0x00);

    /* FX07: delay_timer is not modified */
    cpu = make_cpu();
    cpu.delay_timer = 0xAB;
    exec_ld_vx_dt(&cpu, opcode(0xF3, 0x07));
    ASSERT("LD_VX_DT: delay_timer unchanged", cpu.delay_timer == 0xAB);

    /* FX15: write VX into delay_timer */
    cpu = make_cpu();
    cpu.v_registers[V4] = 0x10;
    exec_ld_dt_vx(&cpu, opcode(0xF4, 0x15));   /* LD DT, V4 */
    ASSERT("LD_DT_VX: delay_timer = V4 (0x10)", cpu.delay_timer == 0x10);

    /* FX15: VX = 0 clears delay_timer */
    cpu = make_cpu();
    cpu.delay_timer = 0x77;
    cpu.v_registers[V0] = 0x00;
    exec_ld_dt_vx(&cpu, opcode(0xF0, 0x15));
    ASSERT("LD_DT_VX: delay_timer = 0 when VX = 0", cpu.delay_timer == 0x00);

    /* FX15: VX is not modified */
    cpu = make_cpu();
    cpu.v_registers[V5] = 0x55;
    exec_ld_dt_vx(&cpu, opcode(0xF5, 0x15));
    ASSERT("LD_DT_VX: V5 unaffected", cpu.v_registers[V5] == 0x55);

    /* FX18: write VX into sound_timer */
    cpu = make_cpu();
    cpu.v_registers[V6] = 0x08;
    exec_ld_st_vx(&cpu, opcode(0xF6, 0x18));   /* LD ST, V6 */
    ASSERT("LD_ST_VX: sound_timer = V6 (0x08)", cpu.sound_timer == 0x08);

    /* FX18: VX = 0xFF -> sound_timer = 0xFF */
    cpu = make_cpu();
    cpu.v_registers[VF] = 0xFF;
    exec_ld_st_vx(&cpu, opcode(0xFF, 0x18));
    ASSERT("LD_ST_VX: sound_timer = 0xFF", cpu.sound_timer == 0xFF);

    /* FX18: VX is not modified */
    cpu = make_cpu();
    cpu.v_registers[V3] = 0x12;
    exec_ld_st_vx(&cpu, opcode(0xF3, 0x18));
    ASSERT("LD_ST_VX: V3 unaffected", cpu.v_registers[V3] == 0x12);

    /* FX18: delay_timer is not touched by sound_timer write */
    cpu = make_cpu();
    cpu.delay_timer = 0x42;
    cpu.v_registers[V1] = 0x0F;
    exec_ld_st_vx(&cpu, opcode(0xF1, 0x18));
    ASSERT("LD_ST_VX: delay_timer unaffected", cpu.delay_timer == 0x42);
}

/* ------------------------------------------------------------------ */
/*  FX55  exec_ld_i_vx  (store V0..VX into memory[I..I+X])            */
/*  FX65  exec_ld_vx_i  (load  V0..VX from memory[I..I+X])            */
/* ------------------------------------------------------------------ */
static void test_exec_ld_i_vx(void)
{
    Cpu cpu;
    int i;

    /* Normal: store V0..V3 at I = 0x300 */
    cpu = make_cpu();
    cpu.index_register = 0x300;
    cpu.v_registers[V0] = 0x11;
    cpu.v_registers[V1] = 0x22;
    cpu.v_registers[V2] = 0x33;
    cpu.v_registers[V3] = 0x44;
    exec_ld_i_vx(&cpu, opcode(0xF3, 0x55));   /* LD [I], V3 */
    ASSERT("LD_I_VX: mem[I+0] = V0 (0x11)", cpu.memory[0x300] == 0x11);
    ASSERT("LD_I_VX: mem[I+1] = V1 (0x22)", cpu.memory[0x301] == 0x22);
    ASSERT("LD_I_VX: mem[I+2] = V2 (0x33)", cpu.memory[0x302] == 0x33);
    ASSERT("LD_I_VX: mem[I+3] = V3 (0x44)", cpu.memory[0x303] == 0x44);

    /* Partial range: X = 0 stores only V0 */
    cpu = make_cpu();
    cpu.index_register = 0x200;
    cpu.v_registers[V0] = 0xAB;
    cpu.v_registers[V1] = 0xCD;   /* should NOT be stored */
    exec_ld_i_vx(&cpu, opcode(0xF0, 0x55));   /* LD [I], V0 */
    ASSERT("LD_I_VX X=0: mem[I+0] = V0 (0xAB)",  cpu.memory[0x200] == 0xAB);
    ASSERT("LD_I_VX X=0: mem[I+1] not written",   cpu.memory[0x201] == 0x00);

    /* Out-of-bounds: I near end of RAM -- iterations past end are silently skipped */
    cpu = make_cpu();
    cpu.index_register = RAM_MEMORY - 2;   /* only indices 0 and 1 are valid */
    cpu.v_registers[V0] = 0x01;
    cpu.v_registers[V1] = 0x02;
    cpu.v_registers[V2] = 0x03;   /* I+2 == RAM_MEMORY -> out of bounds */
    exec_ld_i_vx(&cpu, opcode(0xF2, 0x55));   /* LD [I], V2 */
    ASSERT("LD_I_VX OOB: mem[I+0] written",     cpu.memory[RAM_MEMORY - 2] == 0x01);
    ASSERT("LD_I_VX OOB: mem[I+1] written",     cpu.memory[RAM_MEMORY - 1] == 0x02);
    /* V2 write silently skipped -- no crash, no write beyond RAM */

    /* Memory beyond the stored range is untouched */
    cpu = make_cpu();
    cpu.index_register = 0x400;
    cpu.v_registers[V0] = 0x77;
    exec_ld_i_vx(&cpu, opcode(0xF0, 0x55));
    ASSERT("LD_I_VX: mem[I+1] not clobbered", cpu.memory[0x401] == 0x00);

    /* Full range: store all 16 registers (X = F) */
    cpu = make_cpu();
    cpu.index_register = 0x300;
    for (i = 0; i < 16; i++) cpu.v_registers[i] = (uint8_t)(i + 1);
    exec_ld_i_vx(&cpu, opcode(0xFF, 0x55));
    ASSERT("LD_I_VX full: mem[I+0]  == 1",  cpu.memory[0x300] == 1);
    ASSERT("LD_I_VX full: mem[I+15] == 16", cpu.memory[0x30F] == 16);
}

static void test_exec_ld_vx_i(void)
{
    Cpu cpu;
    int i;

    /* Normal: load V0..V3 from memory at I = 0x300 */
    cpu = make_cpu();
    cpu.index_register = 0x300;
    cpu.memory[0x300] = 0xAA;
    cpu.memory[0x301] = 0xBB;
    cpu.memory[0x302] = 0xCC;
    cpu.memory[0x303] = 0xDD;
    exec_ld_vx_i(&cpu, opcode(0xF3, 0x65));   /* LD V3, [I] */
    ASSERT("LD_VX_I: V0 = mem[I+0] (0xAA)", cpu.v_registers[V0] == 0xAA);
    ASSERT("LD_VX_I: V1 = mem[I+1] (0xBB)", cpu.v_registers[V1] == 0xBB);
    ASSERT("LD_VX_I: V2 = mem[I+2] (0xCC)", cpu.v_registers[V2] == 0xCC);
    ASSERT("LD_VX_I: V3 = mem[I+3] (0xDD)", cpu.v_registers[V3] == 0xDD);

    /* Partial range: X = 0 loads only V0 */
    cpu = make_cpu();
    cpu.index_register = 0x200;
    cpu.memory[0x200] = 0x42;
    cpu.v_registers[V1] = 0xFF;   /* should NOT be touched */
    exec_ld_vx_i(&cpu, opcode(0xF0, 0x65));   /* LD V0, [I] */
    ASSERT("LD_VX_I X=0: V0 = mem[I+0] (0x42)", cpu.v_registers[V0] == 0x42);
    ASSERT("LD_VX_I X=0: V1 not overwritten",    cpu.v_registers[V1] == 0xFF);

    /* Out-of-bounds: I near end of RAM -- iterations past end silently skipped */
    cpu = make_cpu();
    cpu.index_register = RAM_MEMORY - 2;
    cpu.memory[RAM_MEMORY - 2] = 0x11;
    cpu.memory[RAM_MEMORY - 1] = 0x22;
    cpu.v_registers[V0] = 0x00;
    cpu.v_registers[V1] = 0x00;
    cpu.v_registers[V2] = 0xEE;   /* should not be overwritten */
    exec_ld_vx_i(&cpu, opcode(0xF2, 0x65));
    ASSERT("LD_VX_I OOB: V0 loaded",           cpu.v_registers[V0] == 0x11);
    ASSERT("LD_VX_I OOB: V1 loaded",           cpu.v_registers[V1] == 0x22);
    ASSERT("LD_VX_I OOB: V2 not overwritten",  cpu.v_registers[V2] == 0xEE);

    /* Registers beyond the loaded range are untouched */
    cpu = make_cpu();
    cpu.index_register = 0x400;
    cpu.memory[0x400] = 0x55;
    cpu.v_registers[V1] = 0xCC;
    exec_ld_vx_i(&cpu, opcode(0xF0, 0x65));
    ASSERT("LD_VX_I: V1 not clobbered", cpu.v_registers[V1] == 0xCC);

    /* Round-trip: store then load gives back the same values */
    cpu = make_cpu();
    cpu.index_register = 0x300;
    for (i = 0; i < 8; i++) cpu.v_registers[i] = (uint8_t)(0x10 + i);
    exec_ld_i_vx(&cpu, opcode(0xF7, 0x55));   /* store V0..V7 */
    for (i = 0; i < 8; i++) cpu.v_registers[i] = 0x00;
    exec_ld_vx_i(&cpu, opcode(0xF7, 0x65));   /* reload V0..V7 */
    ASSERT("LD round-trip: V0 == 0x10", cpu.v_registers[V0] == 0x10);
    ASSERT("LD round-trip: V7 == 0x17", cpu.v_registers[V7] == 0x17);
}

/* ------------------------------------------------------------------ */
/*  FX0A  exec_ld_vx_k  (blocking key-wait: pc -= 2 if no key)         */
/* ------------------------------------------------------------------ */
static void test_exec_ld_vx_k(void)
{
    Cpu cpu;

    /* No key pressed: pc decremented to repeat instruction */
    cpu = make_cpu();
    cpu.pc = 0x202;
    memset(cpu.input_pad, 0, sizeof(cpu.input_pad));
    exec_ld_vx_k(&cpu, opcode(0xF3, 0x0A));   /* LD V3, K */
    ASSERT("LD_VX_K no key: pc decremented by 2", cpu.pc == 0x200);

    /* Key pressed: pc unchanged, register set */
    cpu = make_cpu();
    cpu.pc = 0x202;
    cpu.input_pad[0x5] = 1;  /* key 5 is pressed */
    exec_ld_vx_k(&cpu, opcode(0xF4, 0x0A));   /* LD V4, K */
    ASSERT("LD_VX_K key pressed: pc unchanged", cpu.pc == 0x202);
    ASSERT("LD_VX_K key pressed: V4 == 0x5", cpu.v_registers[V4] == 0x05);
    
    /* Multiple keys pressed: picks the lowest index */
    cpu = make_cpu();
    cpu.pc = 0x202;
    cpu.input_pad[0xA] = 1;
    cpu.input_pad[0xB] = 1;
    exec_ld_vx_k(&cpu, opcode(0xF2, 0x0A));
    ASSERT("LD_VX_K multiple keys: picks lowest index 0xA", cpu.v_registers[V2] == 0x0A);
}

/* ------------------------------------------------------------------ */
/*  FX29  exec_ld_f_vx  (I = font address of digit VX)                 */
/* ------------------------------------------------------------------ */
static void test_exec_ld_f_vx(void)
{
    Cpu cpu;
    
    #define FONT_START_ADDR 0x050

    /* Digit 0 */
    cpu = make_cpu();
    cpu.v_registers[V0] = 0x0;
    exec_ld_f_vx(&cpu, opcode(0xF0, 0x29));
    ASSERT("LD_F_VX 0: I == 0x050", cpu.index_register == FONT_START_ADDR);

    /* Digit F */
    cpu = make_cpu();
    cpu.v_registers[V1] = 0xF;
    exec_ld_f_vx(&cpu, opcode(0xF1, 0x29));
    ASSERT("LD_F_VX F: I == 0x050 + 75", cpu.index_register == FONT_START_ADDR + (0xF * 5));

    /* Value > 0xF is masked to lower nibble */
    cpu = make_cpu();
    cpu.v_registers[V2] = 0x1A; /* masks to 0xA */
    exec_ld_f_vx(&cpu, opcode(0xF2, 0x29));
    ASSERT("LD_F_VX 0x1A (masked): I == 0x050 + 50", cpu.index_register == FONT_START_ADDR + (0xA * 5));
}

/* ------------------------------------------------------------------ */
/*  Suite entry (called from test_main.c)                              */
/* ------------------------------------------------------------------ */
void run_load_tests(void)
{
    printf("\n=== Load/Store Opcodes ===\n");

    RUN_SUITE(test_exec_store);
    RUN_SUITE(test_exec_ld_reg);
    RUN_SUITE(test_exec_load_i);
    RUN_SUITE(test_exec_timers);
    RUN_SUITE(test_exec_ld_i_vx);
    RUN_SUITE(test_exec_ld_vx_i);
    RUN_SUITE(test_exec_ld_vx_k);
    RUN_SUITE(test_exec_ld_f_vx);
}
