
#include "expression.h"

#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            printf("[FAIL] %s\n", msg); \
            return 1; \
        } else { \
            printf("[ OK ] %s\n", msg); \
        } \
    } while (0)

static int test_null_arguments() {
    ErrorInfo err;
    double result;

    ASSERT(expr_parse_infix(NULL, &err) == NULL, "parse_infix(NULL)");
    ASSERT(err.code == ERR_INVALID_ARG, "parse_infix NULL error code");

    ASSERT(expr_evaluate(NULL, NULL, 0, &result, &err) != 0, "evaluate NULL expr");


    Expression *expr = expr_parse_infix("1+2", &err);
    if (expr) {
        ASSERT(expr_evaluate(expr, NULL, 0, NULL, &err) != 0, "evaluate NULL result");
        expr_free(expr);
    }

    ASSERT(expr_simplify(NULL, &err) == NULL, "simplify NULL");

    ASSERT(expr_differentiate(NULL, "x", &err) == NULL, "differentiate NULL");

    return 0;
}

static int test_basic_parse_and_eval() {
    ErrorInfo err;
    double result = 0.0;

    Expression *expr = expr_parse_infix("3 + 4 * 2", &err);
    ASSERT(expr != NULL, "parse simple expression");

    ASSERT(expr_evaluate(expr, NULL, 0, &result, &err) == 0, "evaluate simple expression");

    ASSERT(result != 0.0, "result is not zero");

    expr_free(expr);
    return 0;
}

static int test_string_conversion() {
    ErrorInfo err;

    Expression *expr = expr_parse_infix("1+2", &err);
    ASSERT(expr != NULL, "parse for string conversion");

    char *infix = expr_to_infix_string(expr, &err);
    ASSERT(infix != NULL, "infix string not NULL");

    printf("Infix: %s\n", infix);

    expr_string_free(infix);

    char *prefix = expr_to_prefix_string(expr, &err);
    ASSERT(prefix != NULL, "prefix string not NULL");

    printf("Prefix: %s\n", prefix);

    expr_string_free(prefix);

    expr_free(expr);
    return 0;
}

static int test_simplify_and_diff() {
    ErrorInfo err;

    Expression *expr = expr_parse_infix("x + 0", &err);
    ASSERT(expr != NULL, "parse for simplify");

    Expression *simp = expr_simplify(expr, &err);
    ASSERT(simp != NULL, "simplify returns value");

    Expression *diff = expr_differentiate(expr, "x", &err);
    ASSERT(diff != NULL, "differentiate returns value");

    expr_free(expr);
    expr_free(simp);
    expr_free(diff);

    return 0;
}

static int test_visualization() {
    ErrorInfo err;

    Expression *expr = expr_parse_infix("1+2", &err);
    ASSERT(expr != NULL, "parse for visualization");

    int rc = expr_visualize_dot(expr, "test_ast.dot", &err);
    ASSERT(rc == 0, "visualize dot success");

    expr_free(expr);
    return 0;
}

int main(void) {
    int rc = 0;

    printf("=== Running API tests ===\n\n");

    rc |= test_null_arguments();
    rc |= test_basic_parse_and_eval();
    rc |= test_string_conversion();
    rc |= test_simplify_and_diff();
    rc |= test_visualization();

    printf("\n=== Tests finished ===\n");

    if (rc == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("SOME TESTS FAILED\n");
    }

    return rc;
}
