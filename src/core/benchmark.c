
#define_POSIX_C_SOURCE 199309L

#include<time.h>

#include "expression.h"
#include "expression_core.h"

static double time_diff_ms(struct timespec start, struct timespec end) {
    double sec = (double)(end.tv_sec - start.tv_sec);
    double nsec = (double)(end.tv_nsec - start.tv_nsec);
    return sec * 1000.0 + nsec / 1000000.0;
}

BenchmarkResult benchmark_expression(const char *infix, int iterations, ErrorInfo *err) {
    BenchmarkResult result = {0};
    struct timespec t1, t2;
    int i;

    expr_error_clear(err);

    if (infix == NULL || iterations <= 0) {
        expr_error_set(err, ERR_INVALID_ARG, "invalid arguments for benchmark");
        return result;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    for (i = 0; i < iterations; i++) {
        Expression *expr = expr_parse_infix(infix, err);
        if (expr == NULL) {
            return result;
        }
        expr_free(expr);
    }

    clock_gettime(CLOCK_MONOTONIC, &t2);
    result.parse_time_ms = time_diff_ms(t1, t2);

    Expression *expr = expr_parse_infix(infix, err);
    if (expr == NULL) {
        return result;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);

    for (i = 0; i < iterations; i++) {
        double value;
        if (expr_evaluate(expr, NULL, 0, &value, err) != 0) {
            expr_free(expr);
            return result;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t2);
    result.eval_time_ms = time_diff_ms(t1, t2);

    expr_free(expr);


    result.operations_count = iterations; 
    result.memory_used = 0; 

    return result;
}
