#include "ast.h"


ExprNode* simplify(ExprNode* node) {

    // 1. Базовые случаи
    if (node == NULL)
        return NULL;
    
    switch (node -> type){
        case NODE_NUMBER:
            return node;
        
        case NODE_VARIABLE:
            return node;
        
        case NODE_BINARY:
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
                free_node(node);

                return new_node;
            }

            if (op == '+'){
                if (left->type == NODE_NUMBER && left->data.number == 0){
                    ExprNode *temp = node->data.binary.right;

                    node->data.binary.left = NULL;
                    node->data.binary.right = NULL;

                    free_node(node);
                    return temp;   

                }   
                if (right->type == NODE_NUMBER && right->data.number == 0){
                    ExprNode *temp = node->data.binary.left;

                    node->data.binary.right = NULL;
                    node->data.binary.left = NULL;

                    free_node(node);
                    return temp;   

                }
            }

            if (op == '-'){
                if (left->type == NODE_NUMBER && left->data.number == 0){
                    ExprNode *temp = create_un_node('-', node->data.binary.right);

                    node->data.binary.right = NULL;
                    node->data.binary.left = NULL;

                    free_node(node);
                    return temp;
                }
                
                if (right->type == NODE_NUMBER && right->data.number == 0){
                    ExprNode *temp = node->data.binary.left;

                    node->data.binary.right = NULL;
                    node->data.binary.left = NULL;

                    free_node(node);
                    return temp;
                }
            }


























        case NODE_FUNCTION:
            //
            break;

        case NODE_UNARY:
            node->data.unary.operand = simplify(node->data.unary.operand);
            ExprNode *child = node->data.unary.operand;

            char op = node->data.unary.op;

            if (op == '+'){
                free_node(node);
                return child;
            }

            if (op == '-' && child->type == NODE_NUMBER){
                double value = child->data.number;

                ExprNode *new_node = create_num_node(-value);

                free_node(child);
                free_node(node);

                return new_node;
            }

            if (op == '-' && child->type == NODE_UNARY && child->data.unary.op == '-'){
                ExprNode *grand = child->data.unary.operand;

                free_node(child);
                free_node(node);

                return grand;
            }

            return node;  
        
    }

    // 2. Рекурсивное упрощение детей


    // 3. Применение правил упрощения

    // 4. Возврат узла
}
