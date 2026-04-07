#ifndef EXPRESSION_H
#define EXPRESSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "expression_types.h"

Expression* expr_parse_infix(const char *infix, ErrorInfo *err);

Expression* expr_parse_postfix(const char *postfix, ErrorInfo *err);

int expr_evaluate(const Expression *expr,
                  const ExprVariable *vars,
                  size_t var_count,
                  double *result,
                  ErrorInfo *err);


Expression* expr_simplify(const Expression *expr, ErrorInfo *err);

Expression* expr_differentiate(const Expression *expr, const char *var, ErrorInfo *err);

char* expr_to_infix_string(const Expression *expr, ErrorInfo *err);

char* expr_to_prefix_string(const Expression *expr, ErrorInfo *err);

int expr_visualize_dot(const Expression *expr, const char *filename, ErrorInfo *err);

void expr_evaluate_stepwise(const Expression *expr, StepCallback cb, ErrorInfo *err);

BenchmarkResult benchmark_expression(const char *infix, int iterations, ErrorInfo *err);

bool run_property_tests(int num_tests, ErrorInfo *err);

void expr_free(Expression *expr);

void expr_string_free(char *str);

const char* expr_error_code_to_string(ErrorCode code);

#ifdef __cplusplus
}
#endif

#endif 
