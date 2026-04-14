#include "conversion.h"

void shunting_yard_test() {
    char infix[] = "3+4*2/(1-5)";
    char postfix[100];
    char error_msg[100];
    long num_operations = 0;

    printf("infinix: %s\n", infix);

    int result = infix_to_postfix(infix, postfix, sizeof(postfix), error_msg, &num_operations);

    if (result == 0) {
        printf("Postfix: %s\n", postfix);
        printf("Operation count: %ld\n", num_operations);
    } else {
        printf("Error: %s\n", error_msg);
    }
}

void syntax_check_tests() {
    char error_msg[256];

    const char* expr1 = "2 + 3 * (4 - 1)";
    if (check_expression_syntax(expr1, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr1);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr1, error_msg);
    }

    const char* expr2 = "2 + () * 3";
    if (check_expression_syntax(expr2, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr2);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr2, error_msg);
    }

    const char* expr3 = "2 +- 3";
    if (check_expression_syntax(expr3, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr3);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr3, error_msg);
    }

    const char* expr4 = "(2 + 3 * 4";
    if (check_expression_syntax(expr4, error_msg)) {
        printf("Ok Expression '%s' is valid\n", expr4);
    } else {
        printf("Err Expression '%s' is invalid: %s\n", expr4, error_msg);
    }
}

 int main() {
     printf("INFINIX EVAL TESTS\n");
     char error_msg[256];
     long num_ops;
     double result;

     const char *expr1 = "3 + 4 * 2";
     if (evaluate_infix(expr1, &result, error_msg, &num_ops) == 0) {
         printf("%s = %.2f (Operations: %ld)\n", expr1, result, num_ops);
     } else {
         printf("Error: %s\n", error_msg);
     }

     const char *expr2 = "(3 + 4) * 2";
     if (evaluate_infix(expr2, &result, error_msg, &num_ops) == 0) {
         printf("%s = %.2f (Operations: %ld)\n", expr2, result, num_ops);
     } else {
         printf("Error: %s\n", error_msg);
     }

     const char *expr3 = "10 - 2 * 3 + (4 + 5) / 3";
     if (evaluate_infix(expr3, &result, error_msg, &num_ops) == 0) {
         printf("%s = %.2f (Operations: %ld)\n", expr3, result, num_ops);
     } else {
         printf("Error: %s\n", error_msg);
     }

     const char *expr4 = "2.5 * 3 + 1.5";
     if (evaluate_infix(expr4, &result, error_msg, &num_ops) == 0) {
         printf("%s = %.2f (Operations: %ld)\n", expr4, result, num_ops);
     } else {
         printf("Error: %s\n", error_msg);
     }

     const char *expr5 = "((2 + 3) * (4 - 1)) / 5";
     if (evaluate_infix(expr5, &result, error_msg, &num_ops) == 0) {
         printf("%s = %.2f (Operations: %ld)\n", expr5, result, num_ops);
     } else {
         printf("Error: %s\n", error_msg);
     }

     printf("POSTFIX EVAL TESTS\n");

     char postfix1[] = "3 4 +";
     double result1;
     long ops1;
     char error1[100];

     printf("Postfix: %s\n", postfix1);
     if (evaluate_postfix(postfix1, &result1, error1, &ops1) == 0) {
         printf("Result: %.2f\n", result1);
         printf("Operation counter: %ld\n\n", ops1);
     } else {
         printf("Error: %s\n\n", error1);
     }

     char postfix2[] = "3 4 2 * 1 5 - / +";
     double result2;
     long ops2;
     char error2[100];

     printf("Postfix: %s\n", postfix2);
     if (evaluate_postfix(postfix2, &result2, error2, &ops2) == 0) {
         printf("Result: %.2f\n", result2);
         printf("Operation counter: %ld\n\n", ops2);
     } else {
         printf("Error: %s\n\n", error2);
     }

     char postfix3[] = "2 3 ^";
     double result3;
     long ops3;
     char error3[100];

     printf("Postfix: %s\n", postfix3);
     if (evaluate_postfix(postfix3, &result3, error3, &ops3) == 0) {
         printf("Result: %.2f\n", result3);
         printf("Operation counter: %ld\n\n", ops3);
     } else {
         printf("Error: %s\n\n", error3);
     }

     char postfix4[] = "5 0 /";
     double result4;
     long ops4;
     char error4[100];

     printf("Postfix: %s\n", postfix4);
     if (evaluate_postfix(postfix4, &result4, error4, &ops4) == 0) {
         printf("Result: %.2f\n", result4);
         printf("Operation counter: %ld\n\n", ops4);
     } else {
         printf("Error: %s\n\n", error4);
     }

     char postfix5[] = "5 +";
     double result5;
     long ops5;
     char error5[100];

     printf("Postfix: %s\n", postfix5);
     if (evaluate_postfix(postfix5, &result5, error5, &ops5) == 0) {
         printf("Result: %.2f\n", result5);
         printf("Operation counter: %ld\n\n", ops5);
     } else {
         printf("Error: %s\n\n", error5);
     }

     printf("SHUNTING YARD TESTS\n");
     shunting_yard_test();

     printf("SYNTAX CHECK TESTS\n");
     syntax_check_tests();
     return 0;
 }
