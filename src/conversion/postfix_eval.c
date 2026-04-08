#include "conversion.h"


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


bool is_operator(const char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
            strcmp(token, "^") == 0);
}

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
        if (error_msg) sprintf(error_msg, "Error: Uncnown operand %s", op);
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

    
    char buffer[1024];
    strncpy(buffer, postfix, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    
    char *token = strtok(buffer, " \t\n");

    while (token != NULL) {
       
        if (strlen(token) == 0) {
            token = strtok(NULL, " \t\n");
            continue;
        }

       
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
                
                if (i != 0) {
                    is_number = false;
                    break;
                }
            } else if (!isdigit(token[i])) {
                is_number = false;
                break;
            }
        }

        
        if (is_number) {
            double value = atof(token);
            if (!push_dstack(&stack, value)) {
                if (error_msg) sprintf(error_msg, "Error: stack overflow");
                return -1;
            }
        }
 
        else if (is_operator(token)) {
            
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
        
        else {
            if (error_msg) sprintf(error_msg,
                "Error: unknown tocken '%s'", token);
            return -1;
        }

        token = strtok(NULL, " \t\n");
    }

    if (is_dstack_empty(&stack)) {
        if (error_msg) sprintf(error_msg, "Error: empty stack");
        return -1;
    }

    *result = pop_dstack(&stack);

    if (!is_dstack_empty(&stack)) {
        if (error_msg) sprintf(error_msg,
            "Error: Several values remain in the stack");
        return -1;
    }

    return 0;
}
