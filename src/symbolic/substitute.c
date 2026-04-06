#include "symbolic.h"
#include <string.h>
#include <stdlib.h>

ExprNode* substitute(ExprNode *node, const char *var_name, double value){
    if (node == NULL)
        return NULL;
    
    switch (node->type){

        case NODE_NUMBER:
            return create_num_node(node->data.number);

        case NODE_VARIABLE:
            if (strcmp(node->data.var_name, var_name) == 0)
                return create_num_node(value);
            
            return create_var_node(node->data.var_name);

        case NODE_UNARY: {
            ExprNode* new_operand = substitute(node->data.unary.operand, var_name, value);

            return create_un_node(node->data.unary.op, new_operand);
        }

        case NODE_BINARY:{
            ExprNode* new_left = substitute(node->data.binary.left, var_name, value);
            ExprNode* new_right = substitute(node->data.binary.right, var_name, value);

            return create_bin_node(node->data.binary.op, new_left, new_right);
        }

        case NODE_FUNCTION: {
            int count = node->data.function.arg_count;

            ExprNode** new_args = malloc(sizeof(ExprNode*) *count);

            for (int i = 0; i < count; i++){
                new_args[i] = substitute(node->data.function.args[i], var_name, value);
            }

            return create_func_node(node->data.function.func_name, count, new_args);
        }
    }
    return NULL;
}