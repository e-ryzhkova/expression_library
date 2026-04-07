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