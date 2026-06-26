
#include "disasm.h"

#include <stdio.h>


void disasm(uint16_t op, char *buf, size_t n)
{
    uint16_t nnn = op & 0x0FFF;
    uint8_t  kk  = op & 0x00FF;
    uint8_t  nib = op & 0x000F;
    uint8_t  x   = (op >> 8) & 0xF;
    uint8_t  y   = (op >> 4) & 0xF;

    switch (op >> 12) {
        case 0x0:
            if (op == 0x00E0) snprintf(buf, n, "CLS [exec_clear_screen]");
            else if (op == 0x00EE) snprintf(buf, n, "RET [exec_return]");
            else snprintf(buf, n, "SYS  0x%03X [unhandled]", nnn);
            break;
        case 0x1: snprintf(buf, n, "JP   0x%03X [exec_jump]", nnn); break;
        case 0x2: snprintf(buf, n, "CALL 0x%03X [exec_call]", nnn); break;
        case 0x3: snprintf(buf, n, "SE   V%X, 0x%02X [exec_se_byte]", x, kk); break;
        case 0x4: snprintf(buf, n, "SNE  V%X, 0x%02X [exec_sne_byte]", x, kk); break;
        case 0x5: snprintf(buf, n, "SE   V%X, V%X [exec_se_reg]", x, y); break;
        case 0x6: snprintf(buf, n, "LD   V%X, 0x%02X [exec_store]", x, kk); break;
        case 0x7: snprintf(buf, n, "ADD  V%X, 0x%02X [exec_add]", x, kk); break;
        case 0x8:
            switch (nib) {
                case 0x0: snprintf(buf, n, "LD   V%X, V%X [exec_ld_reg]", x, y); break;
                case 0x1: snprintf(buf, n, "OR   V%X, V%X [exec_or]", x, y); break;
                case 0x2: snprintf(buf, n, "AND  V%X, V%X [exec_and]", x, y); break;
                case 0x3: snprintf(buf, n, "XOR  V%X, V%X [exec_xor]", x, y); break;
                case 0x4: snprintf(buf, n, "ADD  V%X, V%X [exec_add_reg]", x, y); break;
                case 0x5: snprintf(buf, n, "SUB  V%X, V%X [exec_sub]", x, y); break;
                case 0x6: snprintf(buf, n, "SHR  V%X [exec_shr]", x); break;
                case 0x7: snprintf(buf, n, "SUBN V%X, V%X [exec_subn]", x, y); break;
                case 0xE: snprintf(buf, n, "SHL  V%X [exec_shl]", x); break;
                default:  snprintf(buf, n, "??? 0x%04X [unhandled]", op);
            }
            break;
        case 0x9: snprintf(buf, n, "SNE  V%X, V%X [exec_sne_reg]", x, y); break;
        case 0xA: snprintf(buf, n, "LD   I, 0x%03X [exec_load_i]", nnn); break;
        case 0xB: snprintf(buf, n, "JP   V0, 0x%03X [exec_jump_offset]", nnn); break;
        case 0xC: snprintf(buf, n, "RND  V%X, 0x%02X [exec_rand]", x, kk); break;
        case 0xD: snprintf(buf, n, "DRW  V%X, V%X, %u [exec_draw]", x, y, nib); break;
        case 0xE:
            if (kk == 0x9E) snprintf(buf, n, "SKP  V%X [exec_skp]", x);
            else if (kk == 0xA1) snprintf(buf, n, "SKNP V%X [exec_sknp]", x);
            else snprintf(buf, n, "??? 0x%04X [unhandled]", op);
            break;
        case 0xF:
            switch (kk) {
                case 0x07: snprintf(buf, n, "LD   V%X, DT [exec_ld_vx_dt]", x); break;
                case 0x0A: snprintf(buf, n, "LD   V%X, K [exec_ld_vx_k]", x); break;
                case 0x15: snprintf(buf, n, "LD   DT, V%X [exec_ld_dt_vx]", x); break;
                case 0x18: snprintf(buf, n, "LD   ST, V%X [exec_ld_st_vx]", x); break;
                case 0x1E: snprintf(buf, n, "ADD  I, V%X [exec_add_i]", x); break;
                case 0x29: snprintf(buf, n, "LD   F, V%X [exec_ld_f_vx]", x); break;
                case 0x33: snprintf(buf, n, "LD   B, V%X [exec_ld_b_vx]", x); break;
                case 0x55: snprintf(buf, n, "LD   [I], V%X [exec_ld_i_vx]", x); break;
                case 0x65: snprintf(buf, n, "LD   V%X, [I] [exec_ld_vx_i]", x); break;
                default:   snprintf(buf, n, "??? 0x%04X [unhandled]", op);
            }
            break;
        default: snprintf(buf, n, "??? 0x%04X [unhandled]", op);
    }
}

void trace_opcode(const Cpu *cpu, uint16_t opcode)
{
    char m[48];

    disasm(opcode, m, sizeof m);
    fprintf(stderr,
        "%04X: %-32s  I=%03X SP=%u DT=%02X ST=%02X  "
        "V[%02X %02X %02X %02X %02X %02X %02X %02X "
        "%02X %02X %02X %02X %02X %02X %02X %02X]\n",
        cpu->pc, m, cpu->index_register, cpu->sp,
        cpu->delay_timer, cpu->sound_timer,
        cpu->v_registers[0],  cpu->v_registers[1],  cpu->v_registers[2],
        cpu->v_registers[3],  cpu->v_registers[4],  cpu->v_registers[5],
        cpu->v_registers[6],  cpu->v_registers[7],  cpu->v_registers[8],
        cpu->v_registers[9],  cpu->v_registers[10], cpu->v_registers[11],
        cpu->v_registers[12], cpu->v_registers[13], cpu->v_registers[14],
        cpu->v_registers[15]);
}
