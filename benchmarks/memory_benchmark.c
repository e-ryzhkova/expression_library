#include "expression.h"

int main(void) {
    ErrorInfo err;
    BenchmarkResult result;

    const char *expressions[] = {
        "1+2",
        "3 + 4 * 2 / (1 - 5)",
        "(1 + 2) * (3 + 4) * (5 - 2)",
        "x^2 + 2*x + 1",
        "sin(x) + cos(x) + sqrt(4)"
    };

    int iterations = 500;
    int count = (int)(sizeof(expressions) / sizeof(expressions[0]));
    int i;

    printf("=== Memory Benchmark ===\n\n");

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

        printf("Expression  : %s\n", expressions[i]);
        printf("Iterations  : %d\n", iterations);
        printf("Memory used : %zu bytes\n", result.memory_used);
        printf("\n");
    }

    return 0;
}

