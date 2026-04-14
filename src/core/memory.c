#include "expression.h"
#include "expression_core.h"
#include "ast.h"

Expression* expr_create_from_ast(ExprNode *root) {
    Expression *expr;

    if (root == NULL) {
        return NULL;
    }

    expr = (Expression*)malloc(sizeof(Expression));
    if (expr == NULL) {
        return NULL;
    }

    expr->root = root;
    return expr;
}

void expr_string_free_internal(char *str) {
    free(str);
}

void expr_string_free(char *str) {
    expr_string_free_internal(str);
}

void expr_free(Expression *expr) {
    if (expr == NULL) {
        return;
    }

    if (expr->root != NULL) {
        expr_free_ast(expr->root);
        expr->root = NULL;
    }

    free(expr);
}

void expr_free_ast(ExprNode *root) {
    if (root == NULL) {
        return;
    }

    free_ast(root);
}

// // ExprNode* expr_clone_ast(const ExprNode *root) {
// //     if (root == NULL) {
// //         return NULL;
// //     }

// //     return clone_ast(root);
// }
