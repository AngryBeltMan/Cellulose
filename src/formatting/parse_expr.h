#pragma once
#include <stdlib.h>

#include "../client.h"
#include "parse_bytecode.h"

typedef enum __attribute__((__packed__)) {
    // End of expression
    TOKEN_EOE = 0,
    TOKEN_X,
    TOKEN_Y,
    TOKEN_LPAR,
    TOKEN_RPAR,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_NUM,
    TOKEN_SPACE,
    // used to get a value on the spreadsheet given a coordinate EX: 1,2
    TOKEN_COMMA,
    TOKEN_COLON,
} token_t;


// evaluates the expr returning a number
instructions_t compileExpr(char* input, size_t *char_index);
