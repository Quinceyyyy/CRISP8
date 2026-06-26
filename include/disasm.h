
#ifndef DISASM_H
    #define DISASM_H

    #include <stddef.h>
    #include <stdint.h>

    #include "cpu.h"

    /* Decode `op` into a human mnemonic written to `buf` (size `n`).
       Pure: no CPU state, no globals, no I/O. */
    void disasm(uint16_t op, char *buf, size_t n);

    /* Per-instruction trace: mnemonic + CPU state to stderr.
       Call BEFORE pc advances so the logged address is the instr's own. */
    void trace_opcode(const Cpu *cpu, uint16_t opcode);

#endif
