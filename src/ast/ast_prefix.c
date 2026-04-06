#include "ast.h"

int pre_recursive(const ExprNode* node, char* output, size_t size, int pos) {
    char str[2048] = "";
    if (node->type == NODE_NUMBER) {
        double num = node->data.number;
        if (size > pos) {
            int len = snprintf(output + pos, size - pos, "%f", num);
            return len + pos;
        }
        else
            return -1;
    }

    if (node->type == NODE_VARIABLE) {
        char* var = node->data.var_name;
        if (size > pos) {
            int len = snprintf(output + pos, size - pos, "%s", var);
            return len + pos;
        }
        else
            return -1;
    }

    else if (node->type == NODE_BINARY) {
        char bin = node->data.binary.op;
        if (size > pos) {
            output[pos] = bin;
            pos++;
        }
        else
            return -1;
        if (size > pos)
            output[pos] = ' ';
        else
            return -1;
        ExprNode* left_child = node->data.binary.left;
        ExprNode* right_child = node->data.binary.right;
        pos = pre_recursive(left_child, output, size, pos + 1);
        if (size > pos)
            output[pos] = ' ';
        else
            return -1;
        pos = pre_recursive(right_child, output, size, pos + 1);
        if (size > pos) {
            output[pos] = '\0';
            return pos;
        }
        else
            return -1;
    }

    else if (node->type == NODE_UNARY) {
        char un = node->data.unary.op;
        if (size > pos) {
            output[pos] = un;
            pos++;
        }
        if (size > pos)
            output[pos] = ' ';
        else
            return -1;
        ExprNode* child = node->data.unary.operand;
        pos = pre_recursive(child, output, size, pos + 1);
        if (size > pos) {
            output[pos] = '\0';
            return pos;
        }
        else
            return -1;
    }

    else if (node->type == NODE_FUNCTION) {
        char* func_name = node->data.function.func_name;
        if (size > pos) {
            int len = snprintf(output + pos, size - pos, "%s", func_name);
            ExprNode** args = node->data.function.args;
            if (!strcmp(func_name, "pow")) {
                if (size > pos + len)
                    output[pos + len] = ' ';
                else
                    return -1;
                pos = pre_recursive(args[0], output, size, pos + len + 1);
                if (size > pos)
                    output[pos] = ' ';
                else
                    return -1;
                pos = pre_recursive(args[1], output, size, pos + 1);
            }
            else if ((!strcmp(func_name, "sin") || !strcmp(func_name, "cos") ||
                !strcmp(func_name, "sqrt") || !strcmp(func_name, "ln"))) {
                if (size > pos + len)
                    output[pos + len] = ' ';
                else
                    return -1;
                pos = pre_recursive(args[0], output, size, pos + len + 1);
            }
            if (size > pos) {
                output[pos] = '\0';
                return pos;
            }
            else
                return -1;
        }
        else
            return -1;
    }
}


int ast_to_prefix(const ExprNode* node, char* output, size_t size) {
    int pos = pre_recursive(node, output, size, 0);
    if (pos == -1)
        return -1;
    else
        return 0;
}




