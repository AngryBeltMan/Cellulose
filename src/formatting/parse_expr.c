// compiles the given expression into bytecode
#include "parse_expr.h"
#include "parse_bytecode.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline void pushNum(instructions_t *instructions, instruction_t *opperation, size_t opp_index, cell_num_t opperand);
static inline void appendOpp(instructions_t *ins,instruction_t *opp);

static token_t getNextToken(char* input, size_t *char_index) {
    return
    ((input[*char_index] == 'x') * TOKEN_X) +
    ((input[*char_index] == 'y') * TOKEN_Y) +
    ((input[*char_index] == ',') * TOKEN_COMMA) +
    ((input[*char_index] == ')') * TOKEN_RPAR) +
    ((input[*char_index] == '(') * TOKEN_LPAR) +
    ((input[*char_index] == '-') * TOKEN_MINUS) +
    ((input[*char_index] == '+') * TOKEN_PLUS) +
    ((input[*char_index] == ' ') * TOKEN_SPACE) +
    ((input[*char_index] == '/') * TOKEN_DIV) +
    ((input[*char_index] == '*') * TOKEN_MULT) +
    ((input[*char_index] == ':') * TOKEN_COLON) +
    ((input[*char_index] >= '0' && input[*char_index] <= '9') * TOKEN_NUM);
}
// evaluates the expr returning a number
instructions_t compileExpr(char* input, size_t *char_index ) {
    token_t tok;
    instructions_t instructions = VEC_NEW(instruction_t);
    // The opperation the lhs and the rhs will be applied with.
    // An array because the program will need to still hold on the the opperation
    // when it encounters a grouped expression that need to be evaluated first.
    instruction_t *opperation = calloc(10, sizeof(instruction_t));
    // the index for the variable opperation. Incremented when evaluating inner parenthesis.
    size_t opp_index = 0;
    for (; (tok = getNextToken(input, char_index)) != TOKEN_EOE; ++*char_index) {
        switch (tok) {
           case TOKEN_X:
               VEC_APPEND(instructions, (instruction_t){.opcode= OP_PUSHX});
               appendOpp(&instructions, &opperation[opp_index]);
               break;
           case TOKEN_Y:
               VEC_APPEND(instructions, (instruction_t){.opcode= OP_PUSHY});
               appendOpp(&instructions, &opperation[opp_index]);
               break;
           case TOKEN_SPACE: continue;
           case TOKEN_COLON: continue;
           case TOKEN_NUM: {
                size_t num_starting_index = *char_index;
                while (getNextToken(input, char_index) == TOKEN_NUM) { ++*char_index; }
                cell_num_t opperand = atof(input + num_starting_index);
                pushNum(&instructions, opperation, opp_index, opperand);
                --*char_index;
           } break;
           case TOKEN_PLUS:  opperation[opp_index].opcode = OP_ADD; break;
           case TOKEN_MINUS:  opperation[opp_index].opcode = OP_SUB; break;
           case TOKEN_MULT:  opperation[opp_index].opcode = OP_MULT; break;
           case TOKEN_DIV:  opperation[opp_index].opcode = OP_DIV; break;
           // Used to get the integer/floating point value of a given cell on the spreadsheet
           // EX: (1,x) + 2
           case TOKEN_COMMA:  opperation[opp_index].opcode = OP_GET_CELL; break;
           case TOKEN_LPAR: ++opp_index; break;
           case TOKEN_RPAR:
                --opp_index;
                if (opperation[opp_index].opcode != OP_NOP) {
                    VEC_APPEND(instructions, opperation[opp_index]);
                    opperation[opp_index].opcode = OP_NOP;
                }
                break;
           // un implemented token
           default: return (instructions_t){ .elements = NULL };
        }
    }
    free(opperation);
    return instructions;
}
static inline void pushNum(instructions_t *instructions, instruction_t *opperation, size_t opp_index, cell_num_t opperand) {
    instruction_t ins = {
        .opperand = opperand,
        .opcode = OP_PUSH
    };
    VEC_APPEND((*instructions), ins);
    appendOpp(instructions, &opperation[opp_index]);
}
// appends the current equation opperation if it exists
// Arguments:
// opp - A pointer to the opperation that will appended. For example, 2 + 3, the instruction appended should be OP_ADD.
static inline void appendOpp(instructions_t *ins,instruction_t *opp) {
    if (opp->opcode != OP_NOP) {
        VEC_APPEND((*ins), *opp);
        opp->opcode = OP_NOP;
    }
}
