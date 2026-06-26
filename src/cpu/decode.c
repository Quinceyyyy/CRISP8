
#include "cpu.h"
#include "opcodes.h"

#include <stdint.h>
#include <stdio.h>


static void handle_prefix_0xF(Cpu *cpu, uint16_t opcode)
{
    uint8_t suffix_byte = opcode & 0x00FF;

    switch (suffix_byte) {
        case OPCODE_F_LD_VX_DT: exec_ld_vx_dt(cpu, opcode);
            break;
        case OPCODE_F_LD_VX_K: exec_ld_vx_k(cpu, opcode);
            break;
        case OPCODE_F_LD_DT_VX: exec_ld_dt_vx(cpu, opcode);
            break;
        case OPCODE_F_LD_ST_VX: exec_ld_st_vx(cpu, opcode);
            break;
        case OPCODE_F_ADD_I_VX: exec_add_i(cpu, opcode);
            break;
        case OPCODE_F_LD_F_VX: exec_ld_f_vx(cpu, opcode);
            break;
        case OPCODE_F_LD_B_VX: exec_ld_b_vx(cpu, opcode);
            break;
        case OPCODE_F_LD_I_VX: exec_ld_i_vx(cpu, opcode);
            break;
        case OPCODE_F_LD_VX_I: exec_ld_vx_i(cpu, opcode);
            break;
        default:
            fprintf(stderr, "[handle_prefix_0xF] Error: Unknown 0xF opcode: 0x%04X.\n", opcode);
            break;
    }
}

static void handle_prefix_0x8(Cpu *cpu, uint16_t opcode)
{
    uint8_t suffix_byte = opcode & 0x000F;

    switch (suffix_byte) {
        case OPCODE_8_LD: exec_ld_reg(cpu, opcode);
            break;
        case OPCODE_8_OR: exec_or(cpu, opcode);
            break;
        case OPCODE_8_AND: exec_and(cpu, opcode);
            break;
        case OPCODE_8_XOR: exec_xor(cpu, opcode);
            break;
        case OPCODE_8_ADD: exec_add_reg(cpu, opcode);
            break;
        case OPCODE_8_SUB: exec_sub(cpu, opcode);
            break;
        case OPCODE_8_SHR: exec_shr(cpu, opcode);
            break;
        case OPCODE_8_SUBN: exec_subn(cpu, opcode);
            break;
        case OPCODE_8_SHL: exec_shl(cpu, opcode);
            break;
        default:
            fprintf(stderr, "[handle_prefix_0x8] Error: Unknown 0x8 opcode: 0x%04X.\n", opcode);
            break;
    }
}

static void handle_prefix_0xE(Cpu *cpu, uint16_t opcode)
{
    uint8_t suffix_byte = opcode & 0x00FF;

    switch (suffix_byte) {
        case OPCODE_E_SKP: exec_skp(cpu, opcode);
            break;
        case OPCODE_E_SKNP: exec_sknp(cpu, opcode);
            break;
        default:
            fprintf(stderr, "[handle_prefix_0xE] Error: Unknown 0xE opcode: 0x%04X.\n", opcode);
            break;
    }
}

static void handle_prefix_0x0(Cpu *cpu, uint16_t opcode)
{
    uint8_t suffix_byte = opcode & 0x00FF;

    switch (suffix_byte) {
        case 0xEE: exec_return(cpu);
            break;
        case 0xE0: exec_clear_screen(cpu);
           break;
        default:
           fprintf(stderr, "[handle_prefix_0x0] Error: Unknown 0x0 opcode: 0x%04X.\n", opcode);
           break;
    }
}

int decode_opcode(Cpu *cpu, uint16_t opcode)
{
    uint16_t prefix_bit = opcode >> 12;

    switch (prefix_bit) {
        case OPCODE_PREFIX_0: handle_prefix_0x0(cpu, opcode);
            break;
        case OPCODE_JUMP: exec_jump(cpu, opcode);
            break;
        case OPCODE_CALL: exec_call(cpu, opcode);
            break;
        case OPCODE_SE_BYTE: exec_se_byte(cpu, opcode);
            break;
        case OPCODE_SNE_BYTE: exec_sne_byte(cpu, opcode);
            break;
        case OPCODE_SE_REG: exec_se_reg(cpu, opcode);
            break;
        case OPCODE_STORE: exec_store(cpu, opcode);
            break;
        case OPCODE_ADD_BYTE: exec_add(cpu, opcode);
            break;
        case OPCODE_PREFIX_8: handle_prefix_0x8(cpu, opcode);
            break;
        case OPCODE_SNE_REG: exec_sne_reg(cpu, opcode);
            break;
        case OPCODE_LOAD_I: exec_load_i(cpu, opcode);
            break;
        case OPCODE_JUMP_OFFSET: exec_jump_offset(cpu, opcode);
            break;
        case OPCODE_RAND: exec_rand(cpu, opcode);
            break;
        case OPCODE_DRAW: exec_draw(cpu, opcode);
            break;
        case OPCODE_PREFIX_E: handle_prefix_0xE(cpu, opcode);
            break;
        case OPCODE_PREFIX_F: handle_prefix_0xF(cpu, opcode);
            break;
        default:
            fprintf(stderr, "[decode_opcode] Error: Unknown opcode prefix found: 0x%04x.\n", prefix_bit);
            // cpu->halted = true;
            break;
    }

    // printf("[decode_opcode] Status: opcode at pc [%d]: 0x%04x\n", cpu->pc, opcode);
    return 0;
}
