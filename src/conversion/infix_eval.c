#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_STACK 100

typedef struct {
    double values[MAX_STACK];
    int top;
} ValueStack;

typedef struct {
    char ops[MAX_STACK];
    int top;
} OpStack;

void push_value(ValueStack *vs, double val) {
    vs->values[++vs->top] = val;
}

double pop_value(ValueStack *vs) {
    return vs->values[vs->top--];
}

void push_op(OpStack *os, char op) {
    os->ops[++os->top] = op;
}

char pop_op(OpStack *os) {
    return os->ops[os->top--];
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double apply_op(double a, double b, char op, long *num_operations) {
    (*num_operations)++;
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

int evaluate_infix(const char *expression, double *result,
                   char *error_msg, long *num_operations) {
    ValueStack vals = { .top = -1 };
    OpStack ops = { .top = -1 };
    *num_operations = 0;

    for (int i = 0; expression[i]; i++) {
        if (isspace(expression[i])) continue;

        if (isdigit(expression[i]) || expression[i] == '.') {
            char *endptr;
            double num = strtod(&expression[i], &endptr);
            push_value(&vals, num);
            i = endptr - expression - 1;
        }
        else if (expression[i] == '(') {
            push_op(&ops, '(');
        }
        else if (expression[i] == ')') {
            while (ops.top >= 0 && ops.ops[ops.top] != '(') {
                double b = pop_value(&vals);
                double a = pop_value(&vals);
                char op = pop_op(&ops);
                push_value(&vals, apply_op(a, b, op, num_operations));
            }
            if (ops.top < 0) {
                strcpy(error_msg, "Mismatched parentheses");
                return -1;
            }
            pop_op(&ops);
        }
        else if (strchr("+-*/", expression[i])) {
            while (ops.top >= 0 && ops.ops[ops.top] != '(' &&
                   precedence(ops.ops[ops.top]) >= precedence(expression[i])) {
                double b = pop_value(&vals);
                double a = pop_value(&vals);
                char op = pop_op(&ops);
                push_value(&vals, apply_op(a, b, op, num_operations));
            }
            push_op(&ops, expression[i]);
        }
        else {
            sprintf(error_msg, "Invalid character: %c", expression[i]);
            return -1;
        }
    }

    while (ops.top >= 0) {
        double b = pop_value(&vals);
        double a = pop_value(&vals);
        char op = pop_op(&ops);
        push_value(&vals, apply_op(a, b, op, num_operations));
    }

    if (vals.top != 0) {
        strcpy(error_msg, "Invalid expression");
        return -1;
    }

    *result = pop_value(&vals);
    return 0;
}

// int main() {
//     char error_msg[256];
//     long num_ops;
//     double result;
//
//     // Пример 1: Простое выражение
//     const char *expr1 = "3 + 4 * 2";
//     if (evaluate_infix(expr1, &result, error_msg, &num_ops) == 0) {
//         printf("%s = %.2f (операций: %ld)\n", expr1, result, num_ops);
//     } else {
//         printf("Ошибка: %s\n", error_msg);
//     }
//
//     // Пример 2: Со скобками
//     const char *expr2 = "(3 + 4) * 2";
//     if (evaluate_infix(expr2, &result, error_msg, &num_ops) == 0) {
//         printf("%s = %.2f (операций: %ld)\n", expr2, result, num_ops);
//     } else {
//         printf("Ошибка: %s\n", error_msg);
//     }
//
//     // Пример 3: Сложное выражение
//     const char *expr3 = "10 - 2 * 3 + (4 + 5) / 3";
//     if (evaluate_infix(expr3, &result, error_msg, &num_ops) == 0) {
//         printf("%s = %.2f (операций: %ld)\n", expr3, result, num_ops);
//     } else {
//         printf("Ошибка: %s\n", error_msg);
//     }
//
//     // Пример 4: С десятичными числами
//     const char *expr4 = "2.5 * 3 + 1.5";
//     if (evaluate_infix(expr4, &result, error_msg, &num_ops) == 0) {
//         printf("%s = %.2f (операций: %ld)\n", expr4, result, num_ops);
//     } else {
//         printf("Ошибка: %s\n", error_msg);
//     }
//
//     // Пример 5: Вложенные скобки
//     const char *expr5 = "((2 + 3) * (4 - 1)) / 5";
//     if (evaluate_infix(expr5, &result, error_msg, &num_ops) == 0) {
//         printf("%s = %.2f (операций: %ld)\n", expr5, result, num_ops);
//     } else {
//         printf("Ошибка: %s\n", error_msg);
//     }
//
//     return 0;
// }