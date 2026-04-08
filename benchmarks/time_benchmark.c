#include "expression.h"

int main(void) {
    ErrorInfo err;
    BenchmarkResult result;

    const char *expressions[] = {
        "3 + 4 * 2 / (1 - 5)",
        "2 ^ 3 + 4 * 5 - 6 / 2",
        "(1 + 2) * (3 + 4) * (5 - 2)",
        "sin(3.14 / 2) + cos(0)",
        "x^2 + 2*x + 1"
    };

    int iterations = 1000;
    int count = (int)(sizeof(expressions) / sizeof(expressions[0]));
    int i;

    printf("=== Time Benchmark ===\n\n");

    for (i = 0; i < count; i++) {
        err.code = ERR_OK;
        err.message[0] = '\0';

        result = benchmark_expression(expressions[i], iterations, &err);

        if (err.code != ERR_OK) {
            printf("Expression: %s\n", expressions[i]);
            printf("Error [%s]: %s\n\n",
                   expr_error_code_to_string(err.code),
                   err.message);
            continue;
        }

        printf("Expression       : %s\n", expressions[i]);
        printf("Iterations       : %d\n", iterations);
        printf("Parse time (ms)  : %.6f\n", result.parse_time_ms);
        printf("Eval time (ms)   : %.6f\n", result.eval_time_ms);
        printf("Operations count : %ld\n", result.operations_count);
        printf("Memory used      : %zu bytes\n", result.memory_used);
        printf("\n");
    }

    return 0;
}

