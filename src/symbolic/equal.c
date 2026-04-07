#include "symbolic.h"
#include <string.h>

bool expression_equal(ExprNode *a, ExprNode *b) {
    if (a == NULL && b == NULL)
        return true;

    if (a == NULL || b == NULL)
        return false;

    if (a->type != b->type)
        return false;

    switch (a->type) {
        case NODE_NUMBER:
            return a->data.number == b->data.number;

        case NODE_VARIABLE:
            return strcmp(a->data.var_name, b->data.var_name) == 0;

        case NODE_UNARY:
            return a->data.unary.op == b->data.unary.op &&
                   expression_equal(a->data.unary.operand,
                                    b->data.unary.operand);

        case NODE_BINARY: {
            char op = a->data.binary.op;

            bool direct =
                op == b->data.binary.op &&
                expression_equal(a->data.binary.left,  b->data.binary.left) &&
                expression_equal(a->data.binary.right, b->data.binary.right);

            if (direct)
                return true;

            if (op == '+' || op == '*') {
                bool swapped =
                    op == b->data.binary.op &&
                    expression_equal(a->data.binary.left,  b->data.binary.right) &&
                    expression_equal(a->data.binary.right, b->data.binary.left);

                return swapped;
            }

            return false;
        }

        case NODE_FUNCTION: {
            if (strcmp(a->data.function.func_name,
                       b->data.function.func_name) != 0)
                return false;

            if (a->data.function.arg_count != b->data.function.arg_count)
                return false;

            for (int i = 0; i < a->data.function.arg_count; i++) {
                if (!expression_equal(a->data.function.args[i],
                                      b->data.function.args[i]))
                    return false;
            }

            return true;
        }
    }

    return false;
}
