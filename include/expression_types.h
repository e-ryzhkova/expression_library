#ifndef EXPRESSION_TYPES_H
#define EXPRESSION_TYPES_H

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Expression Expression;

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

typedef struct {
    ErrorCode code;          
    char message[256];       
} ErrorInfo;

typedef struct {
    const char *name;      
    double value;          
} ExprVariable;

typedef struct {
    double parse_time_ms;    
    double eval_time_ms;     
    long operations_count;   
    size_t memory_used;      
} BenchmarkResult;

typedef void (*StepCallback)(const char *step_description,
                             const double *stack,
                             int stack_size);

#endif 

