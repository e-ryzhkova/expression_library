#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "../../include/conversion.h"


void init_stack(Stack *s) {
  s->top = -1;
}

bool is_empty(Stack *s) {
  return s->top == -1;
}

char peek(Stack *s) {
  return s->items[s->top];
}

void push(Stack *s, char c) {
  if (s->top < 99) {
    s->items[++s->top] = c;
  }
}

char pop(Stack *s) {
  if (!is_empty(s)) {
    return s->items[s->top--];
  }
  return '\0';
}

int get_priority(char op) {
  switch (op) {
    case '+': return 1;
    case '-': return 1;
    case '*': return 2;
    case '/': return 2;
    case '^': return 3;
    default: return 0;
  }
}

int get_associative(char op) {
  switch (op) {
    case '+': return 0; // левая ассоциативность
    case '-': return 0;
    case '*': return 0;
    case '/': return 0;
    case '^': return 1; // правая ассоциативность
    default: return 0;
  }
}

bool append_char(char *postfix, size_t postfix_len, char c, size_t *pos) {
  if (*pos + 1 >= postfix_len) {
    return false;
  }
  postfix[*pos] = c;
  (*pos)++;
  postfix[*pos] = '\0';
  return true;
}

int infix_to_postfix(const char *infix, char *postfix, size_t postfix_len,
                     char *error_msg, long *num_operations) {
  Stack stack;
  init_stack(&stack);

  size_t postfix_pos = 0;
  *num_operations = 0;

  if (error_msg) {
    error_msg[0] = '\0';
  }

  postfix[0] = '\0';

  for (int i = 0; infix[i] != '\0'; i++) {
    char token = infix[i];

    // Пропускаем пробелы
    if (token == ' ') continue;

    // Если это цифра (целое число)
    if (isdigit(token)) {
      // Записываем все цифры числа
      while (isdigit(infix[i]) || infix[i] == '.') {
        if (!append_char(postfix, postfix_len, infix[i], &postfix_pos)) {
          if (error_msg) sprintf(error_msg, "Буфер постфиксной строки переполнен");
          return -1;
        }
        i++;
        if (infix[i] == '\0') break;
      }
      i--; // Возвращаемся на последний обработанный символ
      if (!append_char(postfix, postfix_len, ' ', &postfix_pos)) {
        if (error_msg) sprintf(error_msg, "Буфер постфиксной строки переполнен");
        return -1;
      }
      continue;
    }

    // Операторы
    if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') {
      // Обработка унарного минуса/плюса
      if ((token == '-' || token == '+') &&
          (i == 0 || infix[i - 1] == '(' || strchr("+-*/^", infix[i - 1]))) {
        // Унарный оператор - обрабатываем как отдельный токен
        if (!append_char(postfix, postfix_len, '0', &postfix_pos) ||
            !append_char(postfix, postfix_len, ' ', &postfix_pos)) {
          if (error_msg) sprintf(error_msg, "Буфер переполнен");
          return -1;
        }
      }

      while (!is_empty(&stack) && peek(&stack) != '(') {
        char top = peek(&stack);
        int p1 = get_priority(top);
        int p2 = get_priority(token);

        if ((p2 < p1) || (p2 == p1 && get_associative(token) == 0)) {
          pop(&stack);
          if (!append_char(postfix, postfix_len, top, &postfix_pos) ||
              !append_char(postfix, postfix_len, ' ', &postfix_pos)) {
            if (error_msg) sprintf(error_msg, "Буфер переполнен");
            return -1;
          }
          (*num_operations)++;
        } else {
          break;
        }
      }
      push(&stack, token);
      continue;
    }

    // Открывающая скобка
    if (token == '(') {
      push(&stack, token);
      continue;
    }

    // Закрывающая скобка
    if (token == ')') {
      while (!is_empty(&stack) && peek(&stack) != '(') {
        char op = pop(&stack);
        if (!append_char(postfix, postfix_len, op, &postfix_pos) ||
            !append_char(postfix, postfix_len, ' ', &postfix_pos)) {
          if (error_msg) sprintf(error_msg, "Буфер переполнен");
          return -1;
        }
        (*num_operations)++;
      }

      if (is_empty(&stack)) {
        if (error_msg) sprintf(error_msg, "Непарная закрывающая скобка");
        return -1;
      }

      pop(&stack); // Удаляем '('
      continue;
    }

    // Неизвестный символ
    if (error_msg) {
      sprintf(error_msg, "Неизвестный символ: %c", token);
    }
    return -1;
  }

  // Выгружаем оставшиеся операторы из стека
  while (!is_empty(&stack)) {
    char op = pop(&stack);
    if (op == '(') {
      if (error_msg) sprintf(error_msg, "Непарная открывающая скобка");
      return -1;
    }
    if (!append_char(postfix, postfix_len, op, &postfix_pos) ||
        !append_char(postfix, postfix_len, ' ', &postfix_pos)) {
      if (error_msg) sprintf(error_msg, "Буфер переполнен");
      return -1;
    }
    (*num_operations)++;
  }

  // Удаляем последний пробел, если он есть
  if (postfix_pos > 0 && postfix[postfix_pos - 1] == ' ') {
    postfix[postfix_pos - 1] = '\0';
  }

  return 0;
}

// int main() {
//   char infix[] = "3+4*2/(1-5)";
//   char postfix[100];
//   char error_msg[100];
//   long num_operations = 0;
//
//   printf("infinix: %s\n", infix);
//
//   int result = infix_to_postfix(infix, postfix, sizeof(postfix), error_msg, &num_operations);
//
//   if (result == 0) {
//     printf("Postfix: %s\n", postfix);
//     printf("Operation count: %ld\n", num_operations);
//   } else {
//     printf("Ошибка: %s\n", error_msg);
//   }
//
//   return 0;
// }
