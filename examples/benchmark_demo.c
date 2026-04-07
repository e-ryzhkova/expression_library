#include "expression.h"

int main(void) {
    ErrorInfo err;
    BenchmarkResult bench;

    err.code = ERR_OK;
    err.message[0] = '\0';

    bench = benchmark_expression("3 + 4 * 2 / (1 - 5)", 1000, &err);

    if (err.code != ERR_OK) {
        printf("Benchmark error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        return 1;
    }

    printf("Benchmark demo\n");
    printf("--------------\n");
    printf("Expression       : %s\n", "3 + 4 * 2 / (1 - 5)");
    printf("Iterations       : %d\n", 1000);
    printf("Parse time (ms)  : %.6f\n", bench.parse_time_ms);
    printf("Eval time (ms)   : %.6f\n", bench.eval_time_ms);
    printf("Operations count : %ld\n", bench.operations_count);
    printf("Memory used      : %zu bytes\n", bench.memory_used);

    return 0;
}

