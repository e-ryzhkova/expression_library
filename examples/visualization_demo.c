
#include <stdio.h>
#include "expression.h"

int main(void) {
    ErrorInfo err;
    Expression *expr = NULL;
    const char *expression_text = "3 + 4 * 2 / (1 - 5)";

    err.code = ERR_OK;
    err.message[0] = '\0';

    expr = expr_parse_infix(expression_text, &err);
    if (expr == NULL) {
        printf("Parse error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        return 1;
    }

    if (expr_visualize_dot(expr, "ast.dot", &err) != 0) {
        printf("Visualization error [%s]: %s\n",
               expr_error_code_to_string(err.code),
               err.message);
        expr_free(expr);
        return 1;
    }

    printf("Visualization demo\n");
    printf("------------------\n");
    printf("Expression: %s\n", expression_text);
    printf("DOT file generated: ast.dot\n");
    printf("To create an image with Graphviz, run:\n");
    printf("  dot -Tpng ast.dot -o ast.png\n");

    expr_free(expr);
    return 0;
}
