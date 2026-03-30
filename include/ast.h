#ifndef AST_H
#define AST_H
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum { NODE_NUMBER, NODE_VARIABLE, NODE_UNARY, NODE_BINARY, NODE_FUNCTION } NodeType;

typedef struct ExprNode {
    NodeType type;
    union {
        double number;
        char* var_name;
        struct {
            struct ExprNode* operand;
            char op;
        } unary;
        struct {
            struct ExprNode* left;
            struct ExprNode* right;
            char op;
        } binary;
        struct {
            struct ExprNode** args;
            int arg_count;
            char* func_name;
        } function;
    } data;
} ExprNode;

ExprNode* build_ast_from_postfix(const char* postfix, char* error_msg);
ExprNode* create_num_node(double value);
ExprNode* create_var_node(char* var);
ExprNode* create_bin_node(char bin, ExprNode* left, ExprNode* right);
ExprNode* create_un_node(char un, ExprNode* operand);
ExprNode* create_func_node(char* func_name, int arg_count, ExprNode** args);

#endif