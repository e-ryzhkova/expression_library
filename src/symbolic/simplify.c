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
            //
            break;

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
