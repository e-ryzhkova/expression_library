#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdbool.h>
#include <stddef.h>
#pragma once

// Структура стека
typedef struct {
  char items[100];
  int top;
} Stack;

void init_stack(Stack *s);
bool is_empty(Stack *s);
char peek(Stack *s);
void push(Stack *s, char c);
char pop(Stack *s);
int get_priority(char op);
int get_associative(char op);
bool append_char(char *postfix, size_t postfix_len, char c, size_t *pos);

/**
* ЗАДАЧА 1.1: Преобразование инфиксной записи в постфиксную (ОПЗ)
*
* @param infix Строка с инфиксным выражением (нуль-терминированная)
* @param postfix Буфер для записи постфиксной строки (должен быть достаточного размера)
* @param postfix_len Максимальная длина буфера postfix
* @param error_msg Буфер для сообщения об ошибке (может быть NULL)
* @return 0 при успехе, -1 при ошибке (синтаксис, переполнение и т.д.)
*/
int infix_to_postfix(const char* infix, char* postfix, size_t postfix_len,
                     char* error_msg, long* num_operations);

/**
* ЗАДАЧА 1.2: Вычисление постфиксного выражения
*
* @param postfix Строка с постфиксным выражением (токены разделены пробелами)
* @param result Указатель для сохранения результата (double)
* @param error_msg Буфер для сообщения об ошибке
* @return 0 при успехе, -1 при ошибке (деление на ноль, недостаточно операндов)
*/

double evaluate_postfix(const char* postfix, char* error_msg, long* num_operations);

/**
 * ЗАДАЧА 1.4: Вычисление инфиксного выражения напрямую (без явного построения постфикса)
 *
 * @param expression Строка с инфиксным выражением
 * @param result Результат
 * @return 0 при успехе, -1 при ошибке
 */
int evaluate_infix(const char *expression, double *result,
                   char *error_msg, long *num_operations);
#define MAX_STACK 100
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