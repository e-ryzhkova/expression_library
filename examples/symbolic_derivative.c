#include "expression.h"

int main(void) {
    ErrorInfo err;
    Expression *expr = NULL;
    Expression *derivative = NULL;
    Expression *simplified = NULL;
    char *source_str = NULL;
    char *derivative_str = NULL;
    char *simplified_str = NULL;

    err.code = ERR_OK;
    err.message[0] = '\0';

    expr = expr_parse_postfix("x 2 ^ 2 x * +", &err);
    if (expr == NULL) {
        printf("Parse error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        return 1;
    }

    derivative = expr_differentiate(expr, "x", &err);
    if (derivative == NULL) {
        printf("Differentiate error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_free(expr);
        return 1;
    }

    simplified = expr_simplify(derivative, &err);
    if (simplified == NULL) {
        printf("Simplify error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_free(derivative);
        expr_free(expr);
        return 1;
    }

    source_str = expr_to_infix_string(expr, &err);
    if (source_str == NULL) {
        printf("String conversion error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_free(simplified);
        expr_free(derivative);
        expr_free(expr);
        return 1;
    }

    derivative_str = expr_to_infix_string(derivative, &err);
    if (derivative_str == NULL) {
        printf("Derivative string error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_string_free(source_str);
        expr_free(simplified);
        expr_free(derivative);
        expr_free(expr);
        return 1;
    }

    simplified_str = expr_to_infix_string(simplified, &err);
    if (simplified_str == NULL) {
        printf("Simplified string error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_string_free(derivative_str);
        expr_string_free(source_str);
        expr_free(simplified);
        expr_free(derivative);
        expr_free(expr);
        return 1;
    }

    printf("Symbolic derivative demo\n");
    printf("------------------------\n");
    printf("Original expression : %s\n", source_str);
    printf("Derivative          : %s\n", derivative_str);
    printf("Simplified result   : %s\n", simplified_str);

    expr_string_free(simplified_str);
    expr_string_free(derivative_str);
    expr_string_free(source_str);

    expr_free(simplified);
    expr_free(derivative);
    expr_free(expr);

    return 0;
}

