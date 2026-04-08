#include "expression.h"

int main(void) {
    ErrorInfo err;
    Expression *expr;
    char *infix = NULL;
    char *prefix = NULL;

    err.code = ERR_OK;
    err.message[0] = '\0';

    expr = expr_parse_infix("sin(3.14 / 2) + x", &err);
    if (expr == NULL) {
        printf("Parse error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        return 1;
    }

    infix = expr_to_infix_string(expr, &err);
    if (infix == NULL) {
        printf("String conversion error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_free(expr);
        return 1;
    }

    prefix = expr_to_prefix_string(expr, &err);
    if (prefix == NULL) {
        printf("Prefix conversion error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_string_free(infix);
        expr_free(expr);
        return 1;
    }

    printf("AST walk demo\n");
    printf("-------------\n");
    printf("Infix form : %s\n", infix);
    printf("Prefix form: %s\n", prefix);

    if (expr_visualize_dot(expr, "ast_walk.dot", &err) != 0) {
        printf("DOT export error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_string_free(prefix);
        expr_string_free(infix);
        expr_free(expr);
        return 1;
    }

    printf("DOT file saved to: ast_walk.dot\n");
    printf("You can render it with:\n");
    printf("  dot -Tpng ast_walk.dot -o ast_walk.png\n");

    expr_string_free(prefix);
    expr_string_free(infix);
    expr_free(expr);

    return 0;
}


