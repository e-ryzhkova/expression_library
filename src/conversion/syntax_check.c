#include <string.h>
#include <ctype.h>
#include <sec_api/stdio_s.h>

/**
 * ЗАДАЧА 1.3: Проверка баланса скобок и базовой структуры выражения
 *
 * @param expression Строка с выражением
 * @param error_msg Буфер для сообщения об ошибке (не менее 256 байт)
 * @return 1 если скобки сбалансированы и нет заведомо неверных конструкций
 */
int check_expression_syntax(const char *expression, char *error_msg) {
    if (expression == NULL || error_msg == NULL) {
        return 0;
    }

    int parentheses_balance = 0;
    int length = strlen(expression);

    if (length == 0) {
        strcpy(error_msg, "Empty expression");
        return 0;
    }

    for (int i = 0; i < length; i++) {
        char ch = expression[i];
        char next_ch = (i + 1 < length) ? expression[i + 1] : '\0';
        char prev_ch = (i > 0) ? expression[i - 1] : '\0';

        if (ch == '(') {
            parentheses_balance++;

            if (next_ch == ')') {
                strcpy(error_msg, "Empty parentheses '()' are not allowed");
                return 0;
            }
        }
        else if (ch == ')') {
            if (parentheses_balance == 0) {
                strcpy(error_msg, "Unmatched closing parenthesis ')'");
                return 0;
            }
            parentheses_balance--;
        }

        if (ch == '+' && next_ch == '+') {
            strcpy(error_msg, "Invalid operator sequence '++'");
            return 0;
        }

        if (ch == '-' && next_ch == '-') {
            strcpy(error_msg, "Invalid operator sequence '--'");
            return 0;
        }

        if (ch == '*' && next_ch == '*') {
            strcpy(error_msg, "Invalid operator sequence '**'");
            return 0;
        }

        if (ch == '/' && next_ch == '/') {
            strcpy(error_msg, "Invalid operator sequence '//'");
            return 0;
        }

        if (i > 0 &&
            (ch == '+' || ch == '-' || ch == '*' || ch == '/') &&
            (prev_ch == '+' || prev_ch == '-' || prev_ch == '*' || prev_ch == '/')) {
            int is_unary = (i == 1 && (prev_ch == '(')) ||
                           (i == 1 && (prev_ch == '+' || prev_ch == '-')) ||
                           (i == 0);

            if (!is_unary) {
                printf_s("Consecutive operators '%c%c'\n", prev_ch, ch);
                return 0;
            }
        }
    }
    if (parentheses_balance != 0) {
        strcpy(error_msg, "Unmatched opening parenthesis '('");
        return 0;
    }

    strcpy(error_msg, "OK");
    return 1;
}

#include <stdio.h>

int main() {
    char error_msg[256];

    //  Корректное выражение
    const char* expr1 = "2 + 3 * (4 - 1)";
    if (check_expression_syntax(expr1, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr1);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr1, error_msg);
    }

    //  Пустые скобки
    const char* expr2 = "2 + () * 3";
    if (check_expression_syntax(expr2, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr2);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr2, error_msg);
    }

    // Два оператора подряд
    const char* expr3 = "2 +- 3";
    if (check_expression_syntax(expr3, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr3);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr3, error_msg);
    }

    // Незакрытая скобка
    const char* expr4 = "(2 + 3 * 4";
    if (check_expression_syntax(expr4, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr4);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr4, error_msg);
    }

    return 0;
}