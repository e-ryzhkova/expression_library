#include "expression.h"

int main(void) {
    ErrorInfo err;
    Expression *expr;
    double result = 0.0;

    err.code = ERR_OK;
    err.message[0] = '\0';

    expr = expr_parse_infix("3 + 4 * 2 / (1 - 5)", &err);
    if (expr == NULL) {
        printf("Parse error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        return 1;
    }

    if (expr_evaluate(expr, NULL, 0, &result, &err) != 0) {
        printf("Evaluation error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_free(expr);
        return 1;
    }

    printf("Expression: 3 + 4 * 2 / (1 - 5)\n");
    printf("Result: %f\n", result);

    expr_free(expr);
    return 0;
}

