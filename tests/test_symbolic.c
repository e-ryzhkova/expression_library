<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
=======
>>>>>>> 2629d7066ccb15e203e30b5706ecb6a4df5492ba
#include "symbolic.h"

int tests_passed = 0;
int tests_failed = 0;

void check(int cond, const char *msg) {
    if (cond) {
        tests_passed++;
    } else {
        tests_failed++;
        printf("FAIL: %s\n", msg);
    }
}

ExprNode* num(double x) { return create_num_node(x); }
ExprNode* var(const char *s) { return create_var_node(s); }
ExprNode* add(ExprNode *a, ExprNode *b) { return create_bin_node('+', a, b); }
ExprNode* sub(ExprNode *a, ExprNode *b) { return create_bin_node('-', a, b); }
ExprNode* mul(ExprNode *a, ExprNode *b) { return create_bin_node('*', a, b); }
ExprNode* divi(ExprNode *a, ExprNode *b) { return create_bin_node('/', a, b); }
ExprNode* powe(ExprNode *a, ExprNode *b) { return create_bin_node('^', a, b); }

int main() {

<<<<<<< HEAD
    // simlify tests
=======
>>>>>>> 2629d7066ccb15e203e30b5706ecb6a4df5492ba
    ExprNode *s1 = simplify(add(var("x"), num(0)));
    check(s1->type == NODE_VARIABLE, "simplify: x + 0 → x");

    ExprNode *s2 = simplify(mul(var("x"), num(1)));
    check(s2->type == NODE_VARIABLE, "simplify: x * 1 → x");

    ExprNode *s3 = simplify(mul(var("x"), num(0)));
    check(s3->type == NODE_NUMBER && s3->data.number == 0, "simplify: x * 0 → 0");

    ExprNode *s4 = simplify(add(num(2), num(3)));
    check(s4->type == NODE_NUMBER && s4->data.number == 5, "simplify: 2 + 3 → 5");

<<<<<<< HEAD

    // substitute tests
=======
>>>>>>> 2629d7066ccb15e203e30b5706ecb6a4df5492ba
    ExprNode *sub1 = substitute(add(var("x"), num(3)), "x", 10);
    check(sub1->type == NODE_BINARY &&
          sub1->data.binary.left->type == NODE_NUMBER &&
          sub1->data.binary.left->data.number == 10,
          "substitute: x + 3, x=10");

    ExprNode *sub2 = substitute(mul(var("y"), var("x")), "x", 5);
    check(sub2->data.binary.right->type == NODE_NUMBER &&
          sub2->data.binary.right->data.number == 5,
          "substitute: y * x, x=5");

<<<<<<< HEAD
   
    // differentiate tests
=======

>>>>>>> 2629d7066ccb15e203e30b5706ecb6a4df5492ba
    ExprNode *d1 = differentiate(var("x"), "x");
    check(d1->type == NODE_NUMBER && d1->data.number == 1, "differentiate: d/dx x = 1");

    ExprNode *d2 = differentiate(num(5), "x");
    check(d2->type == NODE_NUMBER && d2->data.number == 0, "differentiate: d/dx 5 = 0");

    ExprNode *d3 = differentiate(add(var("x"), var("x")), "x");
    check(d3->type == NODE_BINARY, "differentiate: d/dx (x + x)");

    ExprNode *d4 = differentiate(mul(var("x"), var("x")), "x");
    check(d4->type == NODE_BINARY, "differentiate: d/dx (x * x)");


<<<<<<< HEAD
    // expression_equal tests
=======
>>>>>>> 2629d7066ccb15e203e30b5706ecb6a4df5492ba
    ExprNode *e1 = add(var("x"), var("y"));
    ExprNode *e2 = add(var("y"), var("x"));
    check(expression_equal(e1, e2), "equal: x + y == y + x");

    ExprNode *e3 = mul(var("x"), var("y"));
    ExprNode *e4 = mul(var("y"), var("x"));
    check(expression_equal(e3, e4), "equal: x * y == y * x");

    ExprNode *e5 = add(var("x"), num(1));
    ExprNode *e6 = add(num(1), var("x"));
    check(expression_equal(e5, e6), "equal: x + 1 == 1 + x");

    ExprNode *e7 = sub(var("x"), var("y"));
    ExprNode *e8 = sub(var("y"), var("x"));
    check(!expression_equal(e7, e8), "equal: x - y != y - x");

<<<<<<< HEAD
    // summary
=======
>>>>>>> 2629d7066ccb15e203e30b5706ecb6a4df5492ba
    printf("\nTests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    if (tests_failed == 0)
        printf("ALL TESTS OK\n");

    return 0;
}
<<<<<<< HEAD
=======

>>>>>>> 2629d7066ccb15e203e30b5706ecb6a4df5492ba

