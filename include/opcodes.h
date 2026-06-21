
#ifndef OPCODES_H
    #define OPCODES_H

    #include "cpu.h"
    #include <stdint.h>

    /* ---- top-level opcode prefixes (first nibble) ---- */
    #define OPCODE_PREFIX_0     0x0   /* CLS / RET / SYS -- needs sub-dispatch on low byte */
    #define OPCODE_JUMP         0x1
    #define OPCODE_CALL         0x2
    #define OPCODE_SE_BYTE      0x3
    #define OPCODE_SNE_BYTE     0x4
    #define OPCODE_SE_REG       0x5
    #define OPCODE_STORE        0x6
    #define OPCODE_ADD_BYTE     0x7
    #define OPCODE_PREFIX_8     0x8   /* arithmetic/logic -- needs sub-dispatch on last nibble */
    #define OPCODE_SNE_REG      0x9
    #define OPCODE_LOAD_I       0xA
    #define OPCODE_JUMP_OFFSET  0xB
    #define OPCODE_RAND         0xC
    #define OPCODE_DRAW         0xD
    #define OPCODE_PREFIX_E     0xE   /* input -- needs sub-dispatch on low byte */
    #define OPCODE_PREFIX_F     0xF   /* timers/memory/misc -- needs sub-dispatch on low byte */

    /* ---- 0x0 prefix (low byte) ---- */
    #define OPCODE_0_CLS        0xE0
    #define OPCODE_0_RET        0xEE

    /* ---- 0x8 prefix (last nibble) ---- */
    #define OPCODE_8_LD         0x0
    #define OPCODE_8_OR         0x1
    #define OPCODE_8_AND        0x2
    #define OPCODE_8_XOR        0x3
    #define OPCODE_8_ADD        0x4
    #define OPCODE_8_SUB        0x5
    #define OPCODE_8_SHR        0x6
    #define OPCODE_8_SUBN       0x7
    #define OPCODE_8_SHL        0xE

    /* ---- 0xE prefix (low byte) ---- */
    #define OPCODE_E_SKP        0x9E
    #define OPCODE_E_SKNP       0xA1

    /* ---- 0xF prefix (low byte) ---- */
    #define OPCODE_F_LD_VX_DT   0x07
    #define OPCODE_F_LD_VX_K    0x0A
    #define OPCODE_F_LD_DT_VX   0x15
    #define OPCODE_F_LD_ST_VX   0x18
    #define OPCODE_F_ADD_I_VX   0x1E
    #define OPCODE_F_LD_F_VX    0x29
    #define OPCODE_F_LD_B_VX    0x33
    #define OPCODE_F_LD_I_VX    0x55
    #define OPCODE_F_LD_VX_I    0x65

    /* FLOW */
    void exec_jump(Cpu *cpu, uint16_t opcode);          /* 1NNN */
    void exec_call(Cpu *cpu, uint16_t opcode);           /* 2NNN */
    void exec_return(Cpu *cpu);                          /* 00EE */
    void exec_se_byte(Cpu *cpu, uint16_t opcode);        /* 3XNN */
    void exec_sne_byte(Cpu *cpu, uint16_t opcode);       /* 4XNN */
    void exec_se_reg(Cpu *cpu, uint16_t opcode);         /* 5XY0 */
    void exec_sne_reg(Cpu *cpu, uint16_t opcode);        /* 9XY0 */
    void exec_jump_offset(Cpu *cpu, uint16_t opcode);    /* BNNN */

    /* LOAD */
    void exec_store(Cpu *cpu, uint16_t opcode);          /* 6XNN  LD Vx, byte */
    void exec_load_i(Cpu *cpu, uint16_t opcode);         /* ANNN  LD I, addr */
    void exec_ld_reg(Cpu *cpu, uint16_t opcode);         /* 8XY0  LD Vx, Vy */
    void exec_ld_vx_dt(Cpu *cpu, uint16_t opcode);       /* FX07 */
    void exec_ld_vx_k(Cpu *cpu, uint16_t opcode);        /* FX0A  blocking key read */
    void exec_ld_dt_vx(Cpu *cpu, uint16_t opcode);       /* FX15 */
    void exec_ld_st_vx(Cpu *cpu, uint16_t opcode);       /* FX18 */
    void exec_ld_f_vx(Cpu *cpu, uint16_t opcode);        /* FX29  font char address */
    void exec_ld_i_vx(Cpu *cpu, uint16_t opcode);        /* FX55  store V0..Vx to mem */
    void exec_ld_vx_i(Cpu *cpu, uint16_t opcode);        /* FX65  load V0..Vx from mem */

    /* ARTHIMETIC */
    void exec_add(Cpu *cpu, uint16_t opcode);            /* 7XNN  ADD Vx, byte (no flag) */
    void exec_or(Cpu *cpu, uint16_t opcode);              /* 8XY1 */
    void exec_and(Cpu *cpu, uint16_t opcode);             /* 8XY2 */
    void exec_xor(Cpu *cpu, uint16_t opcode);             /* 8XY3 */
    void exec_add_reg(Cpu *cpu, uint16_t opcode);         /* 8XY4  sets VF */
    void exec_sub(Cpu *cpu, uint16_t opcode);             /* 8XY5  sets VF */
    void exec_shr(Cpu *cpu, uint16_t opcode);             /* 8XY6  sets VF */
    void exec_subn(Cpu *cpu, uint16_t opcode);            /* 8XY7  sets VF */
    void exec_shl(Cpu *cpu, uint16_t opcode);             /* 8XYE  sets VF */
    void exec_add_i(Cpu *cpu, uint16_t opcode);           /* FX1E */
    void exec_rand(Cpu *cpu, uint16_t opcode);            /* CXNN */
    void exec_ld_b_vx(Cpu *cpu, uint16_t opcode);         /* FX33  BCD -- arguably "misc", filed here since it's math-ish */

    /* DISPLAY */
    void exec_clear_screen(Cpu *cpu);                     /* 00E0 */
    void exec_draw(Cpu *cpu, uint16_t opcode);            /* DXYN */

    /* INPUT */
    void exec_skp(Cpu *cpu, uint16_t opcode);             /* EX9E */
    void exec_sknp(Cpu *cpu, uint16_t opcode);            /* EXA1 */

#endif /* OPCODES_H */