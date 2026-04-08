#include "conversion.h"

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
    case '+': return 0; 
    case '-': return 0;
    case '*': return 0;
    case '/': return 0;
    case '^': return 1; 
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

    if (token == ' ') continue;

    if (isdigit(token)) {
      while (isdigit(infix[i]) || infix[i] == '.') {
        if (!append_char(postfix, postfix_len, infix[i], &postfix_pos)) {
          if (error_msg) sprintf(error_msg, "The postfix line buffer is full");
          return -1;
        }
        i++;
        if (infix[i] == '\0') break;
      }
      i--; 
      if (!append_char(postfix, postfix_len, ' ', &postfix_pos)) {
        if (error_msg) sprintf(error_msg, "The postfix line buffer is full");
        return -1;
      }
      continue;
    }

    if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') {
      
      if ((token == '-' || token == '+') &&
          (i == 0 || infix[i - 1] == '(' || strchr("+-*/^", infix[i - 1]))) {
        
        if (!append_char(postfix, postfix_len, '0', &postfix_pos) ||
            !append_char(postfix, postfix_len, ' ', &postfix_pos)) {
          if (error_msg) sprintf(error_msg, "The buffer is full");
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
            if (error_msg) sprintf(error_msg, "The buffer is full");
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

    if (token == '(') {
      push(&stack, token);
      continue;
    }

    if (token == ')') {
      while (!is_empty(&stack) && peek(&stack) != '(') {
        char op = pop(&stack);
        if (!append_char(postfix, postfix_len, op, &postfix_pos) ||
            !append_char(postfix, postfix_len, ' ', &postfix_pos)) {
          if (error_msg) sprintf(error_msg, "The buffer is full");
          return -1;
        }
        (*num_operations)++;
      }

      if (is_empty(&stack)) {
        if (error_msg) sprintf(error_msg, "An unpaired closing bracket");
        return -1;
      }

      pop(&stack); 
      continue;
    }

    
    if (error_msg) {
      sprintf(error_msg, "Unknown symbol: %c", token);
    }
    return -1;
  }

  
  while (!is_empty(&stack)) {
    char op = pop(&stack);
    if (op == '(') {
      if (error_msg) sprintf(error_msg, "An unpaired closing bracket");
      return -1;
    }
    if (!append_char(postfix, postfix_len, op, &postfix_pos) ||
        !append_char(postfix, postfix_len, ' ', &postfix_pos)) {
      if (error_msg) sprintf(error_msg, "The buffer is full");
      return -1;
    }
    (*num_operations)++;
  }

  
  if (postfix_pos > 0 && postfix[postfix_pos - 1] == ' ') {
    postfix[postfix_pos - 1] = '\0';
  }

  return 0;
}

