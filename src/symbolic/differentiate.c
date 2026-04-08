#include "symbolic.h"

ExprNode* differentiate(ExprNode *node, const char *var_name) {
    if (node == NULL)
        return NULL;

    switch (node->type) {
        case NODE_NUMBER:
            return create_num_node(0);

        case NODE_VARIABLE:
            if (strcmp(node->data.var_name, var_name) == 0)
                return create_num_node(1);

            else
                return create_num_node(0);

        case NODE_UNARY: {
            ExprNode *df = differentiate(node->data.unary.operand, var_name);
            return create_un_node(node->data.unary.op, df);
        }

        case NODE_BINARY: {
            char op = node->data.binary.op;

            ExprNode *f  = node->data.binary.left;
            ExprNode *g  = node->data.binary.right;

            ExprNode *df = differentiate(f, var_name);
            ExprNode *dg = differentiate(g, var_name);

            if (op == '+')
                return create_bin_node('+', df, dg);

            if (op == '-')
                return create_bin_node('-', df, dg);

            if (op == '*') {
                ExprNode *term1 = create_bin_node('*', df, g);
                ExprNode *term2 = create_bin_node('*', f, dg);

                return create_bin_node('+', term1, term2);
            }

            if (op == '/') {
                ExprNode *num1 = create_bin_node('*', df, g);
                ExprNode *num2 = create_bin_node('*', f, dg);

                ExprNode *num  = create_bin_node('-', num1, num2);
                ExprNode *den  = create_bin_node('^', g, create_num_node(2));

                return create_bin_node('/', num, den);
            }

            if (op == '^') {
                if (g->type == NODE_NUMBER) {
                    double n = g->data.number;

                    ExprNode *coef = create_num_node(n);
                    ExprNode *pow  = create_bin_node('^', f, create_num_node(n - 1));

                    return create_bin_node('*', coef, pow);
                }
            }

            return NULL;
        }

        case NODE_FUNCTION: {
            char *name = node->data.function.func_name;

            ExprNode *f = node->data.function.args[0];
            ExprNode *df = differentiate(f, var_name);

            if (strcmp(name, "sin") == 0) {
                ExprNode *cos_f = create_func_node("cos", 1, (ExprNode*[]){ f });

                return create_bin_node('*', cos_f, df);
            }

            if (strcmp(name, "cos") == 0) {
                ExprNode *sin_f = create_func_node("sin", 1, (ExprNode*[]){ f });
                ExprNode *neg_sin = create_un_node('-', sin_f);

                return create_bin_node('*', neg_sin, df);
            }

            if (strcmp(name, "ln") == 0) {
                return create_bin_node('/', df, f);
            }

            return NULL;
        }
    }

    return NULL;
}
