#include "ast.h"

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


ExprNode* build_ast_from_postfix(const char* postfix, char* error_msg) {
    int i = -1;
    char* copy = strdup(postfix);
    ExprNode** stack = (ExprNode**)malloc(2 * strlen(copy) * sizeof(ExprNode*));
    char* token = strtok(copy, " ");
    while (token != NULL) {
        char* check;
        double num = strtod(token, &check);
        if (*check == '\0') {
            i++;
            stack[i] = create_num_node(num);
        }
        else if (token == check) {
            if ((!strcmp(check, "sin") || !strcmp(check, "cos") ||
                !strcmp(check, "sqrt") || !strcmp(check, "ln")) && (i >= 1)) {
                ExprNode** args = (ExprNode**)malloc(sizeof(ExprNode*));
                args[0] = stack[i];
                stack[i] = create_func_node(check, 1, args);
            }
            else {
                if (strchr("+-*/^", check[0]) && (i >= 1)) {
                    i--;
                    stack[i] = create_bin_node(check[0], stack[i], stack[i + 1]);
                }
                else if ((check[0] == 'n' || check[0] == 'p') && (i >= 0))
                    stack[i] = create_un_node(check[0], stack[i]);
                else {
                    int valid = 1;
                    for (int j = 0; check[j]; j++)
                        if (!((check[j] >= 'a' && check[j] <= 'z')
                            || (check[j] >= 'A' && check[j] <= 'Z'))) {
                            valid = 0;
                            break;
                        }
                    if (valid) {
                        i++;
                        stack[i] = create_var_node(check);
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
