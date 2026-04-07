#include "expression.h"
#include "expression_core.h"

void expr_error_clear(ErrorInfo *err) {
    if (err == NULL) {
        return;
    }

    err->code = ERR_OK;
    err->message[0] = '\0';
}

void expr_error_set(ErrorInfo *err, ErrorCode code, const char *message) {
    if (err == NULL) {
        return;
    }

    err->code = code;

    if (message == NULL) {
        err->message[0] = '\0';
        return;
    }

    strncpy(err->message, message, sizeof(err->message) - 1);
    err->message[sizeof(err->message) - 1] = '\0';
}

ErrorCode expr_error_from_message(const char *message) {
    if (message == NULL || message[0] == '\0') {
        return ERR_INTERNAL;
    }

    if (strstr(message, "syntax") != NULL ||
        strstr(message, "parenth") != NULL ||
        strstr(message, "token") != NULL ||
        strstr(message, "operand") != NULL) {
        return ERR_SYNTAX;
    }

    if (strstr(message, "division by zero") != NULL ||
        strstr(message, "divide by zero") != NULL) {
        return ERR_DIVISION_BY_ZERO;
    }

    if (strstr(message, "unknown variable") != NULL) {
        return ERR_UNKNOWN_VARIABLE;
    }

    if (strstr(message, "memory") != NULL ||
        strstr(message, "alloc") != NULL) {
        return ERR_MEMORY;
    }

    if (strstr(message, "invalid argument") != NULL ||
        strstr(message, "null") != NULL) {
        return ERR_INVALID_ARG;
    }

    if (strstr(message, "file") != NULL ||
        strstr(message, "io") != NULL) {
        return ERR_IO;
    }

    return ERR_INTERNAL;
}

const char* expr_error_code_to_string(ErrorCode code) {
    switch (code) {
        case ERR_OK:
            return "ERR_OK";
        case ERR_SYNTAX:
            return "ERR_SYNTAX";
        case ERR_UNKNOWN_VARIABLE:
            return "ERR_UNKNOWN_VARIABLE";
        case ERR_DIVISION_BY_ZERO:
            return "ERR_DIVISION_BY_ZERO";
        case ERR_MEMORY:
            return "ERR_MEMORY";
        case ERR_INVALID_ARG:
            return "ERR_INVALID_ARG";
        case ERR_IO:
            return "ERR_IO";
        case ERR_OVERFLOW:
            return "ERR_OVERFLOW";
        case ERR_INTERNAL:
            return "ERR_INTERNAL";
        default:
            return "ERR_UNKNOWN";
    }
}
