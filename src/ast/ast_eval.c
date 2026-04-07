#include "ast.h"

const struct VARS vars[] = {
    {"x", 3.14},
    {"y", 2.71},
    {"z", 1.0},
    {NULL, 0.0}
};

double post_order(const ExprNode* node, 
    const struct { char* name; double value; } *var_table, 
    char* error_msg) {

    if (error_msg[0] != '\0')
        return;

    double num = node->data.number;
    if (node->type == NODE_NUMBER)
        return num;

    else if (node->type == NODE_VARIABLE) {
        for (int i = 0; var_table[i].name; i++)
            if (!(strcmp(var_table[i].name, node->data.var_name)))
                return var_table[i].value;
        strcpy(error_msg, "ERROR: unknown variable");
        return;
    }

    else if (node->type == NODE_BINARY) {
        char bin = node->data.binary.op;
        ExprNode* left_child = node->data.binary.left;
        ExprNode* right_child = node->data.binary.right;
        double left_value = post_order(left_child, var_table, error_msg);
        double right_value = post_order(right_child, var_table, error_msg);
        if (bin == '+')
            num = left_value + right_value;
        else if (bin == '-')
            num = left_value - right_value;
        else if (bin == '*')
            num = left_value * right_value;
        else if (bin == '/') {
            if (right_value != 0)
                num = left_value / right_value;
            else {
                strcpy(error_msg, "ERROR: division by zero");
                return;
            }
        }
        
        else if (bin == '^')
            num = pow(left_value, right_value);
        return num;



    }
    else if (node->type == NODE_UNARY) {
        char un = node->data.unary.op;
        ExprNode* child = node->data.unary.operand;
        double operand_value = post_order(child, var_table, error_msg);
        if (un == 'n')
            num = -operand_value;
        else if (un == 'p') 
            num = operand_value;
        return num;


    }
    else if (node->type == NODE_FUNCTION) {
        char* func_name = node->data.function.func_name;
        ExprNode** args = node->data.function.args;
        double arg1 = post_order(args[0], var_table, error_msg);
        if (!strcmp(func_name, "sin"))
            num = sin(arg1);
        else if (!strcmp(func_name, "cos"))
            num = cos(arg1);
        else if (!strcmp(func_name, "sqrt"))
            if (arg1 >= 0)
                num = sqrt(arg1);
            else {
                strcpy(error_msg, "ERROR: negative argument of the square root");
                return;
            }
        else if (!strcmp(func_name, "ln"))
            if (arg1 > 0)
                num = log(arg1);
            else {
                strcpy(error_msg, "ERROR: non-positive argument of the logarithm");
                return;
            }
        else if (!strcmp(func_name, "pow")) {
            double arg2 = post_order(args[1], var_table, error_msg);
            num = pow(arg1,arg2);
        }
        return num;
    }
    else
        return num;
}

int evaluate_ast(const ExprNode* node, 
    const struct { char* name; double value; } *var_table,
    double* result, char* error_msg) {
    if (error_msg[0] != '\0')
        return -1;
    else {
        *result = post_order(node, var_table, error_msg);
        if (error_msg[0] != '\0')
            return -1;
    }
    return 0;

}



