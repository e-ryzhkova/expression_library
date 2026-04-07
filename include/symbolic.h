#ifndef SYMBOLIC_H
#define SYMBOLIC_H

#include "ast.h"
#include <stdbool.h>

ExprNode* simplify(ExprNode *node);
ExprNode* substitute(ExprNode *node, const char *var_name, double value);
ExprNode* differentiate(ExprNode *node, const char *var_name);
bool expression_equal(ExprNode *a, ExprNode *b);

#endif

