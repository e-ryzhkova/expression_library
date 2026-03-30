#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

// Структура стека для double значений
typedef struct {
    double items[100];
    int top;
} DoubleStack;

void init_dstack(DoubleStack *s) {
    s->top = -1;
}

bool is_dstack_empty(DoubleStack *s) {
    return s->top == -1;
}

bool push_dstack(DoubleStack *s, double value) {
    if (s->top < 99) {
        s->items[++s->top] = value;
        return true;
    }
    return false;
}

double pop_dstack(DoubleStack *s) {
    if (!is_dstack_empty(s)) {
        return s->items[s->top--];
    }
    return 0.0;
}

double peek_dstack(DoubleStack *s) {
    if (!is_dstack_empty(s)) {
        return s->items[s->top];
    }
    return 0.0;
}

// Проверка, является ли строка оператором
bool is_operator(const char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
            strcmp(token, "^") == 0);
}

// Выполнение бинарной операции
bool apply_operator(const char *op, double a, double b, double *result, char *error_msg) {
    if (strcmp(op, "+") == 0) {
        *result = a + b;
    } else if (strcmp(op, "-") == 0) {
        *result = a - b;
    } else if (strcmp(op, "*") == 0) {
        *result = a * b;
    } else if (strcmp(op, "/") == 0) {
        if (fabs(b) < 1e-12) {
            if (error_msg) sprintf(error_msg, "Error: division by zero");
            return false;
        }
        *result = a / b;
    } else if (strcmp(op, "^") == 0) {
        *result = pow(a, b);
    } else {
        if (error_msg) sprintf(error_msg, "Error: Uuncnown operand %s", op);
        return false;
    }
    return true;
}

int evaluate_postfix(const char *postfix, double *result,
                     char *error_msg, long *num_operations) {
    if (!postfix || !result) {
        if (error_msg) sprintf(error_msg, "Error: null ptr");
        return -1;
    }

    DoubleStack stack;
    init_dstack(&stack);
    *num_operations = 0;

    if (error_msg) {
        error_msg[0] = '\0';
    }

    // Создаем копию строки для разбора
    char buffer[1024];
    strncpy(buffer, postfix, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    // Разбиваем строку на токены по пробелам
    char *token = strtok(buffer, " \t\n");

    while (token != NULL) {
        // Пропускаем пустые токены
        if (strlen(token) == 0) {
            token = strtok(NULL, " \t\n");
            continue;
        }

        // Проверяем, является ли токен числом
        bool is_number = true;
        bool has_decimal_point = false;

        for (int i = 0; token[i] != '\0'; i++) {
            if (token[i] == '.') {
                if (has_decimal_point) {
                    is_number = false;
                    break;
                }
                has_decimal_point = true;
            } else if (token[i] == '-') {
                // Минус допустим только в начале числа
                if (i != 0) {
                    is_number = false;
                    break;
                }
            } else if (!isdigit(token[i])) {
                is_number = false;
                break;
            }
        }

        // Если это число
        if (is_number) {
            double value = atof(token);
            if (!push_dstack(&stack, value)) {
                if (error_msg) sprintf(error_msg, "Error: stack overflow");
                return -1;
            }
        }
        // Если это оператор
        else if (is_operator(token)) {
            // Проверяем, достаточно ли операндов
            if (is_dstack_empty(&stack)) {
                if (error_msg) sprintf(error_msg,
                    "Error: insufficient operands for the operator %s", token);
                return -1;
            }
            double b = pop_dstack(&stack);

            if (is_dstack_empty(&stack)) {
                if (error_msg) sprintf(error_msg,
                    "Error: insufficient operands for the operator %s", token);
                return -1;
            }
            double a = pop_dstack(&stack);

            double op_result;
            if (!apply_operator(token, a, b, &op_result, error_msg)) {
                return -1;
            }

            if (!push_dstack(&stack, op_result)) {
                if (error_msg) sprintf(error_msg, "Error: stack overflow");
                return -1;
            }

            (*num_operations)++;
        }
        // Неизвестный токен
        else {
            if (error_msg) sprintf(error_msg,
                "Error: unknown tocken '%s'", token);
            return -1;
        }

        token = strtok(NULL, " \t\n");
    }

    // Проверяем результат
    if (is_dstack_empty(&stack)) {
        if (error_msg) sprintf(error_msg, "Error: empty stack");
        return -1;
    }

    *result = pop_dstack(&stack);

    // Проверяем, что в стеке остался ровно один элемент
    if (!is_dstack_empty(&stack)) {
        if (error_msg) sprintf(error_msg,
            "Error: Several values remain in the stack");
        return -1;
    }

    return 0;
}

// Пример использования и тестирования
// int main() {
//     // Тест 1: простое выражение
//     char postfix1[] = "3 4 +";
//     double result1;
//     long ops1;
//     char error1[100];
//
//     printf("Postfix: %s\n", postfix1);
//     if (evaluate_postfix(postfix1, &result1, error1, &ops1) == 0) {
//         printf("Result: %.2f\n", result1);
//         printf("Operation counter: %ld\n\n", ops1);
//     } else {
//         printf("Error: %s\n\n", error1);
//     }
//
//     // Тест 2: сложное выражение
//     char postfix2[] = "3 4 2 * 1 5 - / +";
//     double result2;
//     long ops2;
//     char error2[100];
//
//     printf("Postfix: %s\n", postfix2);
//     if (evaluate_postfix(postfix2, &result2, error2, &ops2) == 0) {
//         printf("Result: %.2f\n", result2);
//         printf("Operation counter: %ld\n\n", ops2);
//     } else {
//         printf("Error: %s\n\n", error2);
//     }
//
//     // Тест 3: выражение со степенью
//     char postfix3[] = "2 3 ^";
//     double result3;
//     long ops3;
//     char error3[100];
//
//     printf("Postfix: %s\n", postfix3);
//     if (evaluate_postfix(postfix3, &result3, error3, &ops3) == 0) {
//         printf("Result: %.2f\n", result3);
//         printf("Operation counter: %ld\n\n", ops3);
//     } else {
//         printf("Error: %s\n\n", error3);
//     }
//
//     // Тест 4: деление на ноль
//     char postfix4[] = "5 0 /";
//     double result4;
//     long ops4;
//     char error4[100];
//
//     printf("Postfix: %s\n", postfix4);
//     if (evaluate_postfix(postfix4, &result4, error4, &ops4) == 0) {
//         printf("Result: %.2f\n", result4);
//         printf("Operation counter: %ld\n\n", ops4);
//     } else {
//         printf("Error: %s\n\n", error4);
//     }
//
//     // Тест 5: недостаточно операндов
//     char postfix5[] = "5 +";
//     double result5;
//     long ops5;
//     char error5[100];
//
//     printf("Postfix: %s\n", postfix5);
//     if (evaluate_postfix(postfix5, &result5, error5, &ops5) == 0) {
//         printf("Result: %.2f\n", result5);
//         printf("Operation counter: %ld\n\n", ops5);
//     } else {
//         printf("Error: %s\n\n", error5);
//     }
//
//     return 0;
// }
