#pragma once
#include <stdint.h>
#include "../config_include.h"
#include "../vec.h"
#include "../client.h"

#define STACK_SIZE 256

typedef enum __attribute__((__packed__)) {
    // No opperation
    OP_NOP = 0,

    OP_ADD = 1,
    OP_MULT = 2,
    OP_SUB = 3,
    OP_DIV = 4,
    OP_PUSH = 5,
    OP_PUSHX = 6,
    OP_PUSHY = 7,
    OP_POP = 8,
    // pops two values on the stack then, using the position given by the two stack elements, gets the value of the cell given.
    OP_GET_CELL = 9,
} op_t;

typedef struct {
    op_t opcode;
    cell_num_t opperand;
} instruction_t;

typedef VEC(instruction_t) instructions_t;

// Evaluates the bytecode returning the first item on the stack.
// Arguments:
// @byte_code - A vector of opcodes that will be interpreted.
// @client - Contains all of the spreadsheet cells. Used to run the opcode op_t::OP_GET_CELL. Can be set to NULL if the opcode op_t::OP_GET_CELL isn't inside the arg byte_code.
// @res - A pointer that will be mutated if an error has occurred.
cell_num_t evalBCNum(instructions_t *byte_code, Cellulose *client, int *res, coord_int_t x, coord_int_t y);

// Evaluates the byte code and uses the first two values on the stack as coordinates to return the given cell value on the spreadsheet.
// This function may return a NULL pointer if evaluation of the bytecode goes wrong.
// Arguments:
// @byte_code - A vector of opcodes that will be interpreted.
// @client - Contains all of the spreadsheet cells. Used to get and return a cell based on the bytecodes result.
// @res - A pointer that will be mutated if an error has occurred.
cell_t* evalBCCell(instructions_t *byte_code, Cellulose *client, coord_int_t x, coord_int_t y);

