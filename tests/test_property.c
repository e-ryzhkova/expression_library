#include <stdio.h>
#include <stdbool.h>

#include "expression.h"

#define ASSERT(cond, msg)                     \
    do {                                      \
        if (!(cond)) {                        \
            printf("[FAIL] %s\n", msg);       \
            return 1;                         \
        } else {                              \
            printf("[ OK ] %s\n", msg);       \
        }                                     \
    } while (0)

static int test_invalid_argument(void) {
    ErrorInfo err;

    err.code = ERR_OK;
    err.message[0] = '\0';

    ASSERT(run_property_tests(0, &err) == false,
           "run_property_tests(0) should fail");
    ASSERT(err.code == ERR_INVALID_ARG,
           "run_property_tests(0) should set ERR_INVALID_ARG");

    return 0;
}

static int test_small_property_run(void) {
    ErrorInfo err;

    err.code = ERR_OK;
    err.message[0] = '\0';

    ASSERT(run_property_tests(5, &err) == true,
           "run_property_tests(5) should pass");
    ASSERT(err.code == ERR_OK,
           "successful property test run should keep ERR_OK");

    return 0;
}

static int test_medium_property_run(void) {
    ErrorInfo err;

    err.code = ERR_OK;
    err.message[0] = '\0';

    ASSERT(run_property_tests(20, &err) == true,
           "run_property_tests(20) should pass");
    ASSERT(err.code == ERR_OK,
           "medium property run should keep ERR_OK");

    return 0;
}

int main(void) {
    int rc = 0;

    printf("=== Running property tests API checks ===\n\n");

    rc |= test_invalid_argument();
    rc |= test_small_property_run();
    rc |= test_medium_property_run();

    printf("\n=== Property test checks finished ===\n");

    if (rc == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("SOME TESTS FAILED\n");
    }

    return rc;
}

