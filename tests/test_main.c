
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  Shared harness counters                                            */
/* ------------------------------------------------------------------ */
int tests_run    = 0;
int tests_passed = 0;
int tests_failed = 0;

/* ------------------------------------------------------------------ */
/*  Suite declarations                                                 */
/* ------------------------------------------------------------------ */
void run_arithmetic_tests(void);
void run_flow_tests(void);
void run_load_tests(void);
void run_display_tests(void);
void run_input_tests(void);

/* ------------------------------------------------------------------ */
/*  Entry point                                                        */
/* ------------------------------------------------------------------ */
int main(void)
{
    printf("=== CRISP8 Unit Tests ===\n");

    run_arithmetic_tests();
    run_flow_tests();
    run_load_tests();
    run_display_tests();
    run_input_tests();

    printf("\n========================================\n");
    printf("Total: %d/%d passed", tests_passed, tests_run);
    if (tests_failed > 0)
        printf(", %d FAILED", tests_failed);
    printf("\n========================================\n");

    return tests_failed > 0 ? 1 : 0;
}
