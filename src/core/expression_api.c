
#include "expression.h"
#include "expression_core.h"
#include "conversion.h"
#include "ast.h"
#include "symbolic.h"

#define EXPR_LOCAL_ERROR_SIZE 256
#define EXPR_POSTFIX_BUFFER_SIZE 4096
#define EXPR_STRING_BUFFER_SIZE 4096

static int expr_validate_result_pointer(double *result, ErrorInfo *err) {
    if (result == NULL) {
        expr_error_set(err, ERR_INVALID_ARG, "result pointer is NULL");
        return -1;
    }
    return 0;
}

static int expr_validate_expression_pointer(const Expression *expr, ErrorInfo *err) {
    if (expr == NULL) {
        expr_error_set(err, ERR_INVALID_ARG, "expression is NULL");
        return -1;
    }

    if (expr->root == NULL) {
        expr_error_set(err, ERR_INVALID_ARG, "expression root is NULL");
        return -1;
    }

    return 0;
}

static int expr_validate_string_argument(const char *text, const char *arg_name, ErrorInfo *err) {
    char message[256];

    if (text != NULL) {
        return 0;
    }

    snprintf(message, sizeof(message), "%s is NULL", arg_name);
    expr_error_set(err, ERR_INVALID_ARG, message);
    return -1;
}

static void expr_copy_internal_error(ErrorInfo *err, const char *message) {
    ErrorCode code = expr_error_from_message(message);
    expr_error_set(err, code, message != NULL ? message : "internal error");
}

Expression* expr_parse_infix(const char *infix, ErrorInfo *err) {
    char postfix[EXPR_POSTFIX_BUFFER_SIZE];
    char internal_error[EXPR_LOCAL_ERROR_SIZE] = {0};
    long num_operations = 0;
    ExprNode *root = NULL;
    Expression *expr = NULL;

    expr_error_clear(err);

    if (expr_validate_string_argument(infix, "infix", err) != 0) {
        return NULL;
    }

    if (infix_to_postfix(infix,
                         postfix,
                         sizeof(postfix),
                         internal_error,
                         &num_operations) != 0) {
        expr_copy_internal_error(err, internal_error);
        return NULL;
    }

    root = build_ast_from_postfix(postfix, internal_error);
    if (root == NULL) {
        expr_copy_internal_error(err, internal_error);
        return NULL;
    }

    expr = expr_create_from_ast(root);
    if (expr == NULL) {
        expr_free_ast(root);
        expr_error_set(err, ERR_MEMORY, "failed to allocate Expression");
        return NULL;
    }

    return expr;
}

Expression* expr_parse_postfix(const char *postfix, ErrorInfo *err) {
    char internal_error[EXPR_LOCAL_ERROR_SIZE] = {0};
    ExprNode *root = NULL;
    Expression *expr = NULL;

    expr_error_clear(err);

    if (expr_validate_string_argument(postfix, "postfix", err) != 0) {
        return NULL;
    }

    root = build_ast_from_postfix(postfix, internal_error);
    if (root == NULL) {
        expr_copy_internal_error(err, internal_error);
        return NULL;
    }

    expr = expr_create_from_ast(root);
    if (expr == NULL) {
        expr_free_ast(root);
        expr_error_set(err, ERR_MEMORY, "failed to allocate Expression");
        return NULL;
    }

    return expr;
}

int expr_evaluate(const Expression *expr,
                  const ExprVariable *vars,
                  size_t var_count,
                  double *result,
                  ErrorInfo *err) {
    char internal_error[EXPR_LOCAL_ERROR_SIZE] = {0};
    int rc;

    expr_error_clear(err);

    if (expr_validate_expression_pointer(expr, err) != 0) {
        return -1;
    }

    if (expr_validate_result_pointer(result, err) != 0) {
        return -1;
    }

    (void)var_count;

    rc = evaluate_ast(expr->root,
                      (const struct {char *name; double value;} *)vars,
                      result,
                      internal_error);

    if (rc != 0) {
        expr_copy_internal_error(err, internal_error);
        return -1;
    }

    return 0;
}

Expression* expr_simplify(const Expression *expr, ErrorInfo *err) {
    ExprNode *copy_root = NULL;
    ExprNode *simplified_root = NULL;
    Expression *new_expr = NULL;

    expr_error_clear(err);

    if (expr_validate_expression_pointer(expr, err) != 0) {
        return NULL;
    }

    copy_root = expr_clone_ast(expr->root);
    if (copy_root == NULL) {
        expr_error_set(err, ERR_MEMORY, "failed to clone AST for simplify");
        return NULL;
    }

    simplified_root = simplify(copy_root);
    if (simplified_root == NULL) {
        expr_free_ast(copy_root);
        expr_error_set(err, ERR_INTERNAL, "simplify returned NULL");
        return NULL;
    }

    new_expr = expr_create_from_ast(simplified_root);
    if (new_expr == NULL) {
        expr_free_ast(simplified_root);
        expr_error_set(err, ERR_MEMORY, "failed to allocate Expression");
        return NULL;
    }

    return new_expr;
}

Expression* expr_differentiate(const Expression *expr, const char *var, ErrorInfo *err) {
    ExprNode *copy_root = NULL;
    ExprNode *diff_root = NULL;
    Expression *new_expr = NULL;

    expr_error_clear(err);

    if (expr_validate_expression_pointer(expr, err) != 0) {
        return NULL;
    }

    if (expr_validate_string_argument(var, "var", err) != 0) {
        return NULL;
    }

    copy_root = expr_clone_ast(expr->root);
    if (copy_root == NULL) {
        expr_error_set(err, ERR_MEMORY, "failed to clone AST for differentiation");
        return NULL;
    }

    diff_root = differentiate(copy_root, var);
    if (diff_root == NULL) {
        expr_free_ast(copy_root);
        expr_error_set(err, ERR_INTERNAL, "differentiate returned NULL");
        return NULL;
    }

    new_expr = expr_create_from_ast(diff_root);
    if (new_expr == NULL) {
        expr_free_ast(diff_root);
        expr_error_set(err, ERR_MEMORY, "failed to allocate Expression");
        return NULL;
    }

    return new_expr;
}

char* expr_to_infix_string(const Expression *expr, ErrorInfo *err) {
    expr_error_clear(err);

    if (expr_validate_expression_pointer(expr, err) != 0) {
        return NULL;
    }

    return expr_ast_to_infix_string(expr->root, err);
}

char* expr_to_prefix_string(const Expression *expr, ErrorInfo *err) {
    expr_error_clear(err);

    if (expr_validate_expression_pointer(expr, err) != 0) {
        return NULL;
    }

    return expr_ast_to_prefix_string(expr->root, err);
}
