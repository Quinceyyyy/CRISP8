
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

/* ------------------------------------------------------------------ */
/*  00E0  exec_clear_screen  -- STUB                                   */
/*  DXYN  exec_draw          -- STUB                                   */
/*                                                                     */
/*  Both functions are stubs that do not yet implement their           */
/*  described behaviour (exec_clear_screen does not zero               */
/*  display_buffer; exec_draw does not render any pixels).            */
/*  No assertions are made against unimplemented behaviour.           */
/*  Tests will be added when the implementations are complete.        */
/* ------------------------------------------------------------------ */
static void test_display_stubs_placeholder(void)
{
    /* exec_clear_screen: stub -- does not zero display_buffer yet.
       exec_draw: stub -- does not render pixels yet.
       No assertions; placeholder suite to note pending implementation. */
    printf("  [INFO] exec_clear_screen and exec_draw are stubs"
           " -- no assertions\n");
}

/* ------------------------------------------------------------------ */
/*  Suite entry (called from test_main.c)                              */
/* ------------------------------------------------------------------ */
void run_display_tests(void)
{
    printf("\n=== Display Opcodes ===\n");

    RUN_SUITE(test_display_stubs_placeholder);
}
