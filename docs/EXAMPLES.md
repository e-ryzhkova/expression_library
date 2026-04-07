# 📄 `docs/EXAMPLES.md`

````md id="p2x8s4"
# Examples

В этом разделе приведены примеры использования библиотеки.

---

## 1. Базовое вычисление

```c
#include <stdio.h>
#include "expression.h"

int main(void) {
    ErrorInfo err;
    Expression *expr;
    double result;

    expr = expr_parse_infix("3 + 4 * 2", &err);
    if (expr == NULL) {
        printf("Error: %s\n", err.message);
        return 1;
    }

    if (expr_evaluate(expr, NULL, 0, &result, &err) == 0) {
        printf("Result = %f\n", result);
    }

    expr_free(expr);
    return 0;
}
````

---

## 2. Работа с переменными

```c
ExprVariable vars[] = {
    {"x", 2.0},
    {"y", 3.0}
};

expr_evaluate(expr, vars, 2, &result, &err);
```

---

## 3. Упрощение выражения

```c id="ex1"
Expression *simplified = expr_simplify(expr, &err);
```

---

## 4. Производная

```c id="ex2"
Expression *expr = expr_parse_infix("x^2 + 2*x", &err);
Expression *d = expr_differentiate(expr, "x", &err);
```

---

## 5. Преобразование в строку

```c id="ex3"
char *str = expr_to_infix_string(expr, &err);
printf("%s\n", str);
expr_string_free(str);
```

---

## 6. Префиксная запись

```c id="ex4"
char *prefix = expr_to_prefix_string(expr, &err);
printf("%s\n", prefix);
expr_string_free(prefix);
```

---

## 7. Визуализация AST

```c id="ex5"
expr_visualize_dot(expr, "ast.dot", &err);
```

После этого:

```bash
dot -Tpng ast.dot -o ast.png
```

---

## 8. Бенчмаркинг

```c id="ex6"
BenchmarkResult res = benchmark_expression("3 + 4 * 2", 1000, &err);

printf("Parse time: %f ms\n", res.parse_time_ms);
printf("Eval time : %f ms\n", res.eval_time_ms);
```

---

## 9. Property-based тесты

```c id="ex7"
run_property_tests(10, &err);
```

---

## Общий сценарий

```c id="ex8"
Expression *expr = expr_parse_infix("x^2 + 2*x", &err);

Expression *d = expr_differentiate(expr, "x", &err);
Expression *simp = expr_simplify(d, &err);

char *str = expr_to_infix_string(simp, &err);

printf("%s\n", str);

expr_string_free(str);
expr_free(simp);
expr_free(d);
expr_free(expr);
```




