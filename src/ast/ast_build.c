#include "ast.h"

ExprNode* create_func_node(char* func_name, int arg_count, 
    ExprNode** args) {
    ExprNode* node = malloc(sizeof(ExprNode));
    node->type = NODE_FUNCTION;
    node->data.function.func_name = func_name;
    node->data.function.arg_count = arg_count;
    node->data.function.args = args;
    return node;
}



ExprNode* create_bin_node(char bin, ExprNode* left, 
    ExprNode* right) {
    ExprNode* node = malloc(sizeof(ExprNode));
    node->type = NODE_BINARY;
    node->data.binary.op = bin;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}


ExprNode* create_un_node(char un, ExprNode* operand) {
    ExprNode* node = malloc(sizeof(ExprNode));
    node->type = NODE_UNARY;
    node->data.unary.op = un;
    node->data.unary.operand = operand;
    return node;
}

ExprNode* create_var_node(char* var) {
    ExprNode* node = malloc(sizeof(ExprNode));
    node->type = NODE_VARIABLE;
    node->data.var_name = var;
    return node;
}

ExprNode* create_num_node(double value) {
    ExprNode* node = malloc(sizeof(ExprNode));
    node->type = NODE_NUMBER;
    node->data.number = value;
    return node;
}


char* my_strdup(const char* s) {
    int len = strlen(s) + 1;
    char* copy = (char*)malloc(len*sizeof(char));
    strcpy(copy, s);
    return copy;
}

void free_ast(ExprNode* node) {
    switch (node->type) {
        case NODE_NUMBER:
            break;
        case NODE_VARIABLE:
            free_ast(node->data.var_name);
            break;
        case NODE_UNARY:
            free_ast(node->data.unary.operand);
            break;
        case NODE_BINARY:
            free_ast(node->data.binary.left);
            free_ast(node->data.binary.right);
            break;
        case NODE_FUNCTION:
            free_ast(node->data.function.func_name);
            free_ast(node->data.function.args[0]);
            free_ast(node->data.function.args);
            break;
    }
}



ExprNode* build_ast_from_postfix(const char* postfix, char* error_msg) {
    int i = -1;
    char* copy = my_strdup(postfix);
    ExprNode** stack = (ExprNode**)malloc(2*strlen(copy) * sizeof(ExprNode*));
    char* token = strtok(copy, " ");
    while (token != NULL) {
        char* check;
        double num = strtod(token, &check);
        if (*check == '\0') {
            i++;
            stack[i] = create_num_node(num);
        }
        else if (token == check) {
            if ((!strcmp(token, "sin") || !strcmp(token, "cos") ||
                !strcmp(token, "sqrt") || !strcmp(token, "ln")) && (i >= 0)) {
                ExprNode** args = (ExprNode**)malloc(sizeof(ExprNode*));
                args[0] = stack[i];
                stack[i] = create_func_node(my_strdup(token), 1, args);
            }
            else {
                if (strchr("+-*/^", token[0]) && (i >= 1)) {
                    i--;
                    stack[i] = create_bin_node(token[0], stack[i], stack[i + 1]);
                }
                else if ((token[0] == 'n' || token[0] == 'p') && (i >= 0))
                    stack[i] = create_un_node(token[0], stack[i]);
                else {
                    int valid = 1;
                    for (int j = 0; token[j]; j++)
                        if (!((token[j] >= 'a' && token[j] <= 'z')
                            || (token[j] >= 'A' && token[j] <= 'Z'))) {
                            valid = 0;
                            break;
                        }
                    if (valid) {
                        i++;
                        stack[i] = create_var_node(my_strdup(token));
                    }
                    else {
                        strcpy(error_msg, "incorrect entry");
                        free(stack);
                        free(copy);
                        return NULL;
                    }
                }
            }
        }
        else {
            strcpy(error_msg, "incorrect entry");
            free(stack);
            free(copy);
            return NULL;
        }
        token = strtok(NULL, " ");
    }
    ExprNode* root = NULL;
    if (i == 0)
        root = stack[0];
    else
        strcpy(error_msg, "incorrect entry");

    free(stack);
    free(copy);
    return root;
}





