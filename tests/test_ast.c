#include "ast.h"


int main() {
    double res;
    char output[45] = "";
    char error_msg[256] = "";
    const char* tests[] = {
    "1 2 +",
    "5 3 -",
    "4 5 *", 
    "10 2 /",
    "2 3 ^",
    "2 3 + 4 *",
    "8 2 3 * +",
    "10 2 / 3 +",
    "2 3 ^ 4 *",
    "5 n",
    "3 n 4 *",
    "2 3 + n",
    "x 2 *",
    "x y +",
    "x 5 * y +",
    "x y * z +",
    "x 2 ^ y +",
    "1 sin",
    "0 cos",
    "4 sqrt",
    "2.71828 ln",
    "2 3 pow",
    "1 sin 2 cos +",
    "2 3 pow 4 sqrt +",
    "x 2 ^ y 3 ^ + sqrt",
    "2 3 + 4 * 5 -",
    "x 2 * y 3 * + z 4 * -",
    "0",
    "-5",
    "999999999999999999999.999999999999999",
    "5 n 2 +",
    "1 2 3 + +",
    "",
    "sin",
    "+",
    "1 2 + +",
    "1 0 /",
    "x y + *",
    "pow",
    "2 pow",
    "sqrt -4",
    "ln 0",
    "1 sin 2 + cos ln",
    "u",
    "1 1 1 1 + + + n n"
    };
    for (int i = 0; i < 45; i++) {
        error_msg[0] = '\0';
        const char* postfix = tests[i];
        printf("%s\n\n", postfix);
        ExprNode* root = build_ast_from_postfix(postfix, error_msg);
        if (!root) {
            printf("%s\n", error_msg);
        }
        else {
            int check1 = evaluate_ast(root, vars, &res, error_msg);
            int check2 = ast_to_prefix(root, output, sizeof(output));
            if (!check1) {
                printf("ast_eval\n");
                printf("res: %f\n", res);
                printf("check: %d\n", check1);
            }
            else {
                printf("ast_eval\n");
                printf("%s\n", error_msg);
                printf("check2.3: %d\n", check1);
            }
            printf("\n");
            if (!check2) {
                printf("ast_prefix\n");
                printf("output: %s\n", output);
                printf("check: %d\n", check2);
            }
            else {
                printf("ast_prefix\n");
                printf("ERROR: OVERFLOW\n");
                printf("check2.4: %d\n", check2);
            }
            printf("\n");
            printf("ast_constant_fold\n");
            constant_fold(&root);
            if (root->type == NODE_NUMBER) {
                double const_res = root->data.number;
                printf("res: %f\n", const_res);
            }
            else if (error_msg[0] != '\0')
                printf("%s\n", error_msg);
            else
                printf("ERROR: no constant available\n");
        }
        printf("\n");
        printf("\n");
        printf("\n");
        free_ast(root);
    }
    return 0;
}

    


