#include "ast.h"

void constant_fold(ExprNode** node) {
    double num;
    if ((*node)->type == NODE_BINARY) {
        char bin = (*node)->data.binary.op;
        constant_fold(&((*node)->data.binary.left));
        constant_fold(&((*node)->data.binary.right));
        if ((*node)->data.binary.left->type == NODE_NUMBER
            && (*node)->data.binary.right->type == NODE_NUMBER) {
            double left_value = (*node)->data.binary.left->data.number;
            double right_value = (*node)->data.binary.right->data.number;
            if (bin == '+')
                num = left_value + right_value;
            else if (bin == '-')
                num = left_value - right_value;
            else if (bin == '*')
                num = left_value * right_value;
            else if (bin == '/')
                num = left_value / right_value;
            else if (bin == '^')
                num = pow(left_value, right_value);
            ExprNode* new_node = create_num_node(num);
            free_ast(*node);
            *node = new_node;
        }
    }
    else if ((*node)->type == NODE_UNARY) {
        char un = (*node)->data.unary.op;
        constant_fold(&((*node)->data.unary.operand));
        if ((*node)->data.unary.operand->type == NODE_NUMBER) {
            double operand_value = (*node)->data.unary.operand->data.number;
            if (un == 'n')
                num = -operand_value;
            else if (un == 'p')
                num = operand_value;
            ExprNode* new_node = create_num_node(num);
            free_ast(*node);
            *node = new_node;
        }
    }
    else if ((*node)->type == NODE_FUNCTION) {
        char* func_name = (*node)->data.function.func_name;
        ExprNode** args = (*node)->data.function.args;
        if (strcmp(func_name, "pow")) {
            constant_fold(&((*node)->data.function.args[0]));
            if ((*node)->data.function.args[0]->type == NODE_NUMBER) {
                double arg1 = (*node)->data.function.args[0]->data.number;
                if (!strcmp(func_name, "sin"))
                    num = sin(arg1);
                else if (!strcmp(func_name, "cos"))
                    num = cos(arg1);
                else if (!strcmp(func_name, "sqrt"))
                    num = sqrt(arg1);
                else if (!strcmp(func_name, "ln"))
                    num = log(arg1);
                ExprNode* new_node = create_num_node(num);
                free_ast(*node);
                *node = new_node;
            }
        }
        else {
            constant_fold(&((*node)->data.function.args[0]));
            constant_fold(&((*node)->data.function.args[1]));
            if ((*node)->data.function.args[0]->type == NODE_NUMBER
                && (*node)->data.function.args[1]->type == NODE_NUMBER) {
                double arg1 = (*node)->data.function.args[0]->data.number;
                double arg2 = (*node)->data.function.args[1]->data.number;
                num = pow(arg1, arg2);
                ExprNode* new_node = create_num_node(num);
                free_ast(*node);
                *node = new_node;
            }
        }
    }
}

