
#include "expression.h"
#include "expression_core.h"
#include "ast.h"

#define EXPR_STRING_CAPACITY 4096

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} StringBuilder;

static int sb_init(StringBuilder *sb, size_t capacity) {
    if (sb == NULL || capacity == 0) {
        return -1;
    }

    sb->data = (char *)malloc(capacity);
    if (sb->data == NULL) {
        return -1;
    }

    sb->length = 0;
    sb->capacity = capacity;
    sb->data[0] = '\0';
    return 0;
}

static void sb_free(StringBuilder *sb) {
    if (sb == NULL) {
        return;
    }

    free(sb->data);
    sb->data = NULL;
    sb->length = 0;
    sb->capacity = 0;
}

static int sb_append(StringBuilder *sb, const char *text) {
    size_t text_len;

    if (sb == NULL || text == NULL) {
        return -1;
    }

    text_len = strlen(text);

    if (sb->length + text_len + 1 > sb->capacity) {
        return -1;
    }

    memcpy(sb->data + sb->length, text, text_len + 1);
    sb->length += text_len;
    return 0;
}

static int sb_appendf(StringBuilder *sb, const char *fmt, ...) {
    va_list args;
    int written;
    size_t available;

    if (sb == NULL || fmt == NULL) {
        return -1;
    }

    available = sb->capacity - sb->length;

    va_start(args, fmt);
    written = vsnprintf(sb->data + sb->length, available, fmt, args);
    va_end(args);

    if (written < 0) {
        return -1;
    }

    if ((size_t)written >= available) {
        return -1;
    }

    sb->length += (size_t)written;
    return 0;
}

static char* sb_build_result(StringBuilder *sb) {
    char *result;

    if (sb == NULL || sb->data == NULL) {
        return NULL;
    }

    result = sb->data;
    sb->data = NULL;
    sb->length = 0;
    sb->capacity = 0;
    return result;
}

static int ast_to_infix_recursive(const ExprNode *node, StringBuilder *sb) {
    int i;

    if (node == NULL || sb == NULL) {
        return -1;
    }

    switch (node->type) {
        case NODE_NUMBER:
            return sb_appendf(sb, "%g", node->data.number);

        case NODE_VARIABLE:
            return sb_append(sb, node->data.var_name);

        case NODE_UNARY:
            if (sb_appendf(sb, "(%c", node->data.unary.op) != 0) {
                return -1;
            }
            if (ast_to_infix_recursive(node->data.unary.operand, sb) != 0) {
                return -1;
            }
            return sb_append(sb, ")");

        case NODE_BINARY:
            if (sb_append(sb, "(") != 0) {
                return -1;
            }
            if (ast_to_infix_recursive(node->data.binary.left, sb) != 0) {
                return -1;
            }
            if (sb_appendf(sb, " %c ", node->data.binary.op) != 0) {
                return -1;
            }
            if (ast_to_infix_recursive(node->data.binary.right, sb) != 0) {
                return -1;
            }
            return sb_append(sb, ")");

        case NODE_FUNCTION:
            if (sb_append(sb, node->data.function.func_name) != 0) {
                return -1;
            }
            if (sb_append(sb, "(") != 0) {
                return -1;
            }

            for (i = 0; i < node->data.function.arg_count; ++i) {
                if (i > 0) {
                    if (sb_append(sb, ", ") != 0) {
                        return -1;
                    }
                }

                if (ast_to_infix_recursive(node->data.function.args[i], sb) != 0) {
                    return -1;
                }
            }

            return sb_append(sb, ")");

        default:
            return -1;
    }
}

static int ast_to_prefix_recursive(const ExprNode *node, StringBuilder *sb) {
    int i;

    if (node == NULL || sb == NULL) {
        return -1;
    }

    switch (node->type) {
        case NODE_NUMBER:
            return sb_appendf(sb, "%g", node->data.number);

        case NODE_VARIABLE:
            return sb_append(sb, node->data.var_name);

        case NODE_UNARY:
            if (sb_appendf(sb, "%c ", node->data.unary.op) != 0) {
                return -1;
            }
            return ast_to_prefix_recursive(node->data.unary.operand, sb);

        case NODE_BINARY:
            if (sb_appendf(sb, "%c ", node->data.binary.op) != 0) {
                return -1;
            }
            if (ast_to_prefix_recursive(node->data.binary.left, sb) != 0) {
                return -1;
            }
            if (sb_append(sb, " ") != 0) {
                return -1;
            }
            return ast_to_prefix_recursive(node->data.binary.right, sb);

        case NODE_FUNCTION:
            if (sb_append(sb, node->data.function.func_name) != 0) {
                return -1;
            }
            if (sb_append(sb, " ") != 0) {
                return -1;
            }

            for (i = 0; i < node->data.function.arg_count; ++i) {
                if (i > 0) {
                    if (sb_append(sb, " ") != 0) {
                        return -1;
                    }
                }

                if (ast_to_prefix_recursive(node->data.function.args[i], sb) != 0) {
                    return -1;
                }
            }

            return 0;

        default:
            return -1;
    }
}

char* expr_ast_to_infix_string(const ExprNode *root, ErrorInfo *err) {
    StringBuilder sb;

    expr_error_clear(err);

    if (root == NULL) {
        expr_error_set(err, ERR_INVALID_ARG, "AST root is NULL");
        return NULL;
    }

    if (sb_init(&sb, EXPR_STRING_CAPACITY) != 0) {
        expr_error_set(err, ERR_MEMORY, "failed to allocate string buffer");
        return NULL;
    }

    if (ast_to_infix_recursive(root, &sb) != 0) {
        sb_free(&sb);
        expr_error_set(err, ERR_OVERFLOW, "failed to build infix string");
        return NULL;
    }

    return sb_build_result(&sb);
}

char* expr_ast_to_prefix_string(const ExprNode *root, ErrorInfo *err) {
    StringBuilder sb;

    expr_error_clear(err);

    if (root == NULL) {
        expr_error_set(err, ERR_INVALID_ARG, "AST root is NULL");
        return NULL;
    }

    if (sb_init(&sb, EXPR_STRING_CAPACITY) != 0) {
        expr_error_set(err, ERR_MEMORY, "failed to allocate string buffer");
        return NULL;
    }

    if (ast_to_prefix_recursive(root, &sb) != 0) {
        sb_free(&sb);
        expr_error_set(err, ERR_OVERFLOW, "failed to build prefix string");
        return NULL;
    }

    return sb_build_result(&sb);
}

static int dot_write_node(FILE *fp, const ExprNode *node, int *next_id, int *current_id) {
    int my_id;
    int child_id;
    int i;

    if (fp == NULL || node == NULL || next_id == NULL || current_id == NULL) {
        return -1;
    }

    my_id = (*next_id)++;
    *current_id = my_id;

    switch (node->type) {
        case NODE_NUMBER:
            fprintf(fp, "  node%d [label=\"%g\"];\n", my_id, node->data.number);
            return 0;

        case NODE_VARIABLE:
            fprintf(fp, "  node%d [label=\"%s\"];\n", my_id, node->data.var_name);
            return 0;

        case NODE_UNARY:
            fprintf(fp, "  node%d [label=\"%c\"];\n", my_id, node->data.unary.op);

            if (dot_write_node(fp, node->data.unary.operand, next_id, &child_id) != 0) {
                return -1;
            }

            fprintf(fp, "  node%d -> node%d;\n", my_id, child_id);
            return 0;

        case NODE_BINARY:
            fprintf(fp, "  node%d [label=\"%c\"];\n", my_id, node->data.binary.op);

            if (dot_write_node(fp, node->data.binary.left, next_id, &child_id) != 0) {
                return -1;
            }
            fprintf(fp, "  node%d -> node%d;\n", my_id, child_id);

            if (dot_write_node(fp, node->data.binary.right, next_id, &child_id) != 0) {
                return -1;
            }
            fprintf(fp, "  node%d -> node%d;\n", my_id, child_id);

            return 0;

        case NODE_FUNCTION:
            fprintf(fp, "  node%d [label=\"%s\"];\n", my_id, node->data.function.func_name);

            for (i = 0; i < node->data.function.arg_count; ++i) {
                if (dot_write_node(fp, node->data.function.args[i], next_id, &child_id) != 0) {
                    return -1;
                }
                fprintf(fp, "  node%d -> node%d;\n", my_id, child_id);
            }

            return 0;

        default:
            return -1;
    }
}

int expr_ast_export_dot(const ExprNode *root, const char *filename, ErrorInfo *err) {
    FILE *fp;
    int next_id = 0;
    int root_id = -1;

    expr_error_clear(err);

    if (root == NULL) {
        expr_error_set(err, ERR_INVALID_ARG, "AST root is NULL");
        return -1;
    }

    if (filename == NULL) {
        expr_error_set(err, ERR_INVALID_ARG, "filename is NULL");
        return -1;
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        expr_error_set(err, ERR_IO, "failed to open DOT file for writing");
        return -1;
    }

    fprintf(fp, "digraph AST {\n");
    fprintf(fp, "  node [shape=box];\n");

    if (dot_write_node(fp, root, &next_id, &root_id) != 0) {
        fclose(fp);
        expr_error_set(err, ERR_INTERNAL, "failed to write AST to DOT");
        return -1;
    }

    fprintf(fp, "}\n");

    if (fclose(fp) != 0) {
        expr_error_set(err, ERR_IO, "failed to close DOT file");
        return -1;
    }

    return 0;
}
