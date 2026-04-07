#include "ast.h"
#include "symbolic.h"
#include <string.h>
#include <stdlib.h>

ExprNode* simplify(ExprNode* node) {
    if (node == NULL)
        return NULL;
    
    switch (node -> type){
        case NODE_NUMBER: {
            return node;
        }

        case NODE_VARIABLE: {
            return node;
        } 

        case NODE_UNARY: {
            node->data.unary.operand = simplify(node->data.unary.operand);
            ExprNode *child = node->data.unary.operand;

            char op = node->data.unary.op;

            if (op == '+'){
                node->data.unary.operand = NULL; 
                free_ast(node);
                return child;
            }

            if (op == '-' && child->type == NODE_NUMBER){
                double value = child->data.number;

                ExprNode *new_node = create_num_node(-value);

                free_ast(child);

                node->data.unary.operand = NULL; 
                free_ast(node);

                return new_node;
            }

            if (op == '-' && child->type == NODE_UNARY && child->data.unary.op == '-'){
                ExprNode *grand = child->data.unary.operand;

                free_ast(child);

                node->data.unary.operand = NULL; 
                free_ast(node);

                return grand;
            }

            return node;  
        }
        
        case NODE_BINARY: {
            node->data.binary.left = simplify(node->data.binary.left);
            node->data.binary.right = simplify(node->data.binary.right);

            ExprNode *right = node->data.binary.right;
            ExprNode *left = node->data.binary.left;

            char op = node->data.binary.op;

            if (right->type == NODE_NUMBER && left->type == NODE_NUMBER){
                double result;
                double a = left->data.number; double b = right->data.number;

                switch (op){

                    case '+': result = a + b; break;

                    case '-': result = a - b; break;

                    case '*': result = a * b; break;

                    case '/': result = a / b; break;

                    case '^': result = pow(a, b); break;       
                }

                ExprNode * new_node = create_num_node(result);
                free_ast(node);

                return new_node;
            }


            if (op == '+'){
                if (left->type == NODE_NUMBER && left->data.number == 0){
                    ExprNode *temp = node->data.binary.right;

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;   
                }  

                if (right->type == NODE_NUMBER && right->data.number == 0){      
                    ExprNode *left  = create_bin_node('*', df, create_bin_node('*', NULL, NULL)); 
                    ExprNode *temp = node->data.binary.left;

                    node->data.binary.right = NULL;
                    node->data.binary.left = NULL;

                    free_ast(node);
                    return temp;   
                }
            }


            if (op == '-'){
                if (left->type == NODE_NUMBER && left->data.number == 0){
                    ExprNode *temp = create_un_node('-', node->data.binary.right);

                    node->data.binary.right = NULL;
                    node->data.binary.left = NULL;

                    free_ast(node);
                    return temp;
                }
                
                if (right->type == NODE_NUMBER && right->data.number == 0){
                    ExprNode *temp = node->data.binary.left;

                    node->data.binary.right = NULL;
                    node->data.binary.left = NULL;

                    free_ast(node);
                    return temp;
                }
            }

                
            if (op == '*'){
                if (right->type == NODE_NUMBER && right->data.number == 1){
                    ExprNode *temp = left;

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }
        
                if (left->type == NODE_NUMBER && left->data.number == 1){
                    ExprNode *temp = right;

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }

                if (right->type == NODE_NUMBER && right->data.number == 0){
                    ExprNode *temp = create_num_node(0);

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }

                if (left->type == NODE_NUMBER && left->data.number == 0){
                    ExprNode *temp = create_num_node(0);

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }
            }


            if (op == '/'){
                if (right->type == NODE_NUMBER && right->data.number == 1){
                    ExprNode *temp = left;

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }

                if (left->type == NODE_NUMBER && left->data.number == 0){
                    ExprNode *temp = create_num_node(0);

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }       
            }

        
            if (op == '^'){
                if (right->type == NODE_NUMBER && right->data.number == 1){
                    ExprNode *temp = left;

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }

                if (right->type == NODE_NUMBER && right->data.number == 0){
                    ExprNode *temp = create_num_node(1);

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }

                if (left->type == NODE_NUMBER && left->data.number == 1){
                    ExprNode *temp = create_num_node(1);

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }

                if (left->type == NODE_NUMBER && left->data.number == 0){
                    ExprNode *temp = create_num_node(0);

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_ast(node);
                    return temp;
                }
            }

            return node;
        }

        case NODE_FUNCTION: {
            for (int i = 0; i < node->data.function.arg_count; i++)
                node->data.function.args[i] = simplify(node->data.function.args[i]);

            int n = node->data.function.arg_count;
            for (int i = 0; i < n; i++){
                if (node->data.function.args[i]->type != NODE_NUMBER)
                    return node;
            }

            double vals[n];
            for (int i = 0; i < n; i++)
                vals[i] = node->data.function.args[i]->data.number;

            double result;

            if (strcmp(node->data.function.func_name, "sin") == 0 && n == 1)
                result = sin(vals[0]);
            else if (strcmp(node->data.function.func_name, "cos") == 0 && n == 1)
                result = cos(vals[0]);
            else if (strcmp(node->data.function.func_name, "log") == 0 && n == 2)
                result = log(vals[0]) / log(vals[1]);
            else if (strcmp(node->data.function.func_name, "max") == 0 && n >= 1){
                result = vals[0];
                for (int i = 1; i < n; i++)
                    if (vals[i] > result)
                        result = vals[i];
            }
            else
                return node;

            ExprNode *new_node = create_num_node(result);

            for (int i = 0; i < n; i++)
                node->data.function.args[i] = NULL;

            free_ast(node);
            return new_node;
        }  
    }
}
