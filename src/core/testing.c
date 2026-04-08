
#include "expression.h"
#include "expression_core.h"

static const char *simple_expressions[] = {
    "1+2",
    "3*4+5",
    "10/(2+3)",
    "5-2*3",
    "2^3+1"
};

#define NUM_EXPRESSIONS (sizeof(simple_expressions)/sizeof(simple_expressions[0]))

bool run_property_tests(int num_tests, ErrorInfo *err) {
    int i;

    expr_error_clear(err);

    if (num_tests <= 0) {
        expr_error_set(err, ERR_INVALID_ARG, "num_tests must be > 0");
        return false;
    }

    srand((unsigned int)time(NULL));

    for (i = 0; i < num_tests; i++) {
        const char *expr_text = simple_expressions[rand() % NUM_EXPRESSIONS];

        Expression *expr = expr_parse_infix(expr_text, err);
        if (expr == NULL) {
            printf("[FAIL] parse failed: %s\n", expr_text);
            return false;
        }

        double result;
        if (expr_evaluate(expr, NULL, 0, &result, err) != 0) {
            printf("[FAIL] eval failed: %s\n", expr_text);
            expr_free(expr);
            return false;
        }

        Expression *simplified = expr_simplify(expr, err);
        if (simplified == NULL) {
            printf("[FAIL] simplify failed: %s\n", expr_text);
            expr_free(expr);
            return false;
        }

        expr_free(simplified);
        expr_free(expr);
    }

    printf("[OK] Property tests passed (%d tests)\n", num_tests);
    return true;
}
