#ifndef EXPRESSION_CORE_H
#define EXPRESSION_CORE_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "expression_types.h"
#include "ast.h"


struct Expression {
    ExprNode *root;
};

void expr_error_clear(ErrorInfo *err);

void expr_error_set(ErrorInfo *err, ErrorCode code, const char *message);

ErrorCode expr_error_from_message(const char *message);

Expression* expr_create_from_ast(ExprNode *root);

void expr_free_ast(ExprNode *root);

ExprNode* expr_clone_ast(const ExprNode *root);

void expr_string_free_internal(char *str);

char* expr_ast_to_infix_string(const ExprNode *root, ErrorInfo *err);

char* expr_ast_to_prefix_string(const ExprNode *root, ErrorInfo *err);

int expr_ast_export_dot(const ExprNode *root, const char *filename, ErrorInfo *err);

#endif 

