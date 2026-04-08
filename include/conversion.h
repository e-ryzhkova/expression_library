#ifndef CONVERSION_H
#define CONVERSION_H

#define MAX_STACK 100

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
//#pragma once


typedef struct {
  char items[100];
  int top;
} Stack;

void init_stack(Stack *s);
int is_empty(Stack *s);
char peek(Stack *s);
void push(Stack *s, char c);
char pop(Stack *s);
int get_priority(char op);
int get_associative(char op);
int append_char(char *postfix, size_t postfix_len, char c, size_t *pos);


int infix_to_postfix(const char* infix, char* postfix, size_t postfix_len,
                     char* error_msg, long* num_operations);

int evaluate_postfix(const char* postfix, char* error_msg, long* num_operations);

int check_expression_syntax(const char *expression, char *error_msg);

int evaluate_infix(const char *expression, double *result,
                   char *error_msg, long *num_operations);

typedef struct {
  double values[MAX_STACK];
  int top;
} ValueStack;

typedef struct {
  char ops[MAX_STACK];
  int top;
} OpStack;
void push_value(ValueStack *vs, double val);
double pop_value(ValueStack *vs);
void push_op(OpStack *os, char op);
char pop_op(OpStack *os);
int precedence(char op);
double apply_op(double a, double b, char op, long *num_operations);

#endif
