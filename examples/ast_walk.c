#include "ast.h"

int main() {
    double res;
    char output[8] = "";
    char error_msg[256] = "";
    const char* postfix = "1 2 +";
    ExprNode* root = build_ast_from_postfix(postfix, error_msg);
    if (!root)
        printf("ERROR: %s\n", error_msg);
    else {
        int check1 = evaluate_ast(root, vars, &res, error_msg);
        int check2 = ast_to_prefix(root, output, sizeof(output));
        if (!check1) {
            printf("res: %f\n", res);
            printf("check2.3: %d\n", check1);
        }
        else {
            printf("ERROR: %s\n", error_msg);
            printf("check2.3: %d\n", check1);
        }
        printf("\n");
        if (!check2) {
            printf("output: %s\n", output);
            printf("check2.4: %d\n", check2);
        }
        else {
            printf("ERROR: OVERFLOW\n");
            printf("check2.4: %d\n", check2);
        }
        printf("\n");
        constant_fold(&root);
        if (root->type == NODE_NUMBER) {
            double const_res = root->data.number;
            printf("const res: %f\n", const_res);
        }
        else
            printf("no constant available");
    }
    free_ast(root);
    return 0;
}


