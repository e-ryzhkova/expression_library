
# API Documentation

## Общие типы

### Expression

`Expression` — это основной тип библиотеки, представляющий арифметическое выражение.

```c
typedef struct Expression Expression;
````

Это непрозрачный тип: внутренняя структура скрыта от пользователя библиотеки.

---

### ErrorCode

Коды ошибок, которые используются во всех функциях библиотеки.

```c
typedef enum {
    ERR_OK = 0,
    ERR_SYNTAX,
    ERR_UNKNOWN_VARIABLE,
    ERR_DIVISION_BY_ZERO,
    ERR_MEMORY,
    ERR_INVALID_ARG,
    ERR_IO,
    ERR_OVERFLOW,
    ERR_INTERNAL
} ErrorCode;
```

---

### ErrorInfo

Структура для передачи подробной информации об ошибке.

```c
typedef struct {
    ErrorCode code;
    char message[256];
} ErrorInfo;
```

---

### ExprVariable

Структура для передачи значений переменных при вычислении выражения.

```c
typedef struct {
    const char *name;
    double value;
} ExprVariable;
```

Пример:

```c
ExprVariable vars[] = {
    {"x", 2.0},
    {"y", 3.5}
};
```

---

### BenchmarkResult

Структура результата бенчмаркинга.

```c
typedef struct {
    double parse_time_ms;
    double eval_time_ms;
    long operations_count;
    size_t memory_used;
} BenchmarkResult;
```

---

## Основные функции API

### Парсинг выражений

#### `expr_parse_infix`

Разбирает инфиксное выражение и создаёт объект `Expression`.

```c
Expression* expr_parse_infix(const char *infix, ErrorInfo *err);
```

Параметры:

* `infix` — строка с выражением в инфиксной записи
* `err` — структура для описания ошибки

Возвращает:

* указатель на `Expression` при успехе
* `NULL` при ошибке

Пример:

```c
ErrorInfo err;
Expression *expr = expr_parse_infix("3 + 4 * 2", &err);
```

---

#### `expr_parse_postfix`

Разбирает выражение в постфиксной записи.

```c
Expression* expr_parse_postfix(const char *postfix, ErrorInfo *err);
```

---

### Вычисление выражения

#### `expr_evaluate`

Вычисляет значение выражения с учётом переданных переменных.

```c
int expr_evaluate(const Expression *expr,
                  const ExprVariable *vars,
                  size_t var_count,
                  double *result,
                  ErrorInfo *err);
```

Параметры:

* `expr` — выражение
* `vars` — массив переменных
* `var_count` — количество элементов в массиве `vars`
* `result` — указатель для записи результата
* `err` — информация об ошибке

Возвращает:

* `0` при успехе
* ненулевое значение при ошибке

Пример:

```c
ErrorInfo err;
double result;

ExprVariable vars[] = {
    {"x", 2.0}
};

if (expr_evaluate(expr, vars, 1, &result, &err) == 0) {
    printf("Result = %f\n", result);
}
```

---

### Символьные преобразования

#### `expr_simplify`

Упрощает выражение и возвращает новое выражение.

```c
Expression* expr_simplify(const Expression *expr, ErrorInfo *err);
```

Исходное выражение не изменяется.

---

#### `expr_differentiate`

Строит производную выражения по указанной переменной.

```c
Expression* expr_differentiate(const Expression *expr, const char *var, ErrorInfo *err);
```

Пример:

```c
Expression *d = expr_differentiate(expr, "x", &err);
```

---

### Преобразование в строку

#### `expr_to_infix_string`

Преобразует выражение в инфиксную строку.

```c
char* expr_to_infix_string(const Expression *expr, ErrorInfo *err);
```

Возвращаемую строку нужно освободить через:

```c
expr_string_free(str);
```

---

#### `expr_to_prefix_string`

Преобразует выражение в префиксную запись.

```c
char* expr_to_prefix_string(const Expression *expr, ErrorInfo *err);
```

---

### Визуализация

#### `expr_visualize_dot`

Экспортирует AST в файл формата Graphviz DOT.

```c
int expr_visualize_dot(const Expression *expr, const char *filename, ErrorInfo *err);
```

Пример:

```c
expr_visualize_dot(expr, "ast.dot", &err);
```

После этого можно создать изображение командой:

```bash
dot -Tpng ast.dot -o ast.png
```

---

### Бенчмаркинг

#### `benchmark_expression`

Запускает измерение производительности для заданного выражения.

```c
BenchmarkResult benchmark_expression(const char *infix, int iterations, ErrorInfo *err);
```

Пример:

```c
BenchmarkResult res = benchmark_expression("3 + 4 * 2", 1000, &err);
```

---

### Тестирование свойств

#### `run_property_tests`

Запускает серию property-based тестов.

```c
bool run_property_tests(int num_tests, ErrorInfo *err);
```

---

### Управление памятью

#### `expr_free`

Освобождает выражение.

```c
void expr_free(Expression *expr);
```

Безопасно вызывать с `NULL`.

---

#### `expr_string_free`

Освобождает строку, возвращённую библиотекой.

```c
void expr_string_free(char *str);
```

---

### Работа с ошибками

#### `expr_error_code_to_string`

Преобразует код ошибки в строку.

```c
const char* expr_error_code_to_string(ErrorCode code);
```

Пример:

```c
printf("%s\n", expr_error_code_to_string(err.code));
```

---

## Общая схема использования библиотеки

Типичный сценарий работы:

1. Разобрать выражение через `expr_parse_infix`
2. Вычислить его через `expr_evaluate`
3. При необходимости упростить или продифференцировать
4. Освободить память через `expr_free`

Пример:

```c
#include <stdio.h>
#include "expression.h"

int main(void) {
    ErrorInfo err;
    Expression *expr;
    double result;

    expr = expr_parse_infix("3 + 4 * 2", &err);
    if (expr == NULL) {
        printf("Parse error: %s\n", err.message);
        return 1;
    }

    if (expr_evaluate(expr, NULL, 0, &result, &err) == 0) {
        printf("Result = %f\n", result);
    } else {
        printf("Evaluation error: %s\n", err.message);
    }

    expr_free(expr);
    return 0;
}


