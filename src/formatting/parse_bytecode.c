#include "parse_bytecode.h"
#include <assert.h>
#include <stdio.h>
#include "../messages.h"
// the stack used for evaluating bytecode
static cell_num_t stack[STACK_SIZE];
// Stack pointer. Unlike most real stacks this grows upward.
// Used to parse the bytecode generated from parsing the expression.
static size_t sp = 0;
#define OPCODE_CASE(_op_code_case, _opperation) \
    case _op_code_case: { \
        cell_num_t rhs = stackPop(); \
        cell_num_t lhs = stackPop(); \
        cell_num_t evaluated = lhs _opperation rhs;\
        if (stackPush(evaluated) == -1) { \
            return -1; \
        } \
    } break;
// pops the first item in the stack returning that value
static cell_num_t stackPop() {
    // Check for underflow
    if (sp == 0)
        return 0;
    return stack[--sp];
}
// pushes the argument item onto the stack. Returns negative number if the stack is full.
static int stackPush(cell_num_t item) {
    logToFile("PUSHING ITEM: %f\n", item);
    // overflow check
    stack[sp] = item;
    if (++sp > STACK_SIZE)
        return -1;
    return 0;
}
// Pops the top two items on the stack using them as y and x coordinate values to grab that specific cell in the spreadsheet.
static cell_t* getCell(Cellulose *client) {
    assert(client && "INVALID CLIENT PTR FN: getCell");
    // minus one because the spreadsheet starts at coordinate (1,1)
    coord_int_t y = stackPop() - 1;
    coord_int_t x = stackPop() - 1;
    // check to make sure the cell exist. This also checks for negative coordinates.
    // This is done to prevent a segmentation fault.
    if (!cellExist(client, x, y)) {
        logToFile("%s: [%i] - Cannot access cell, cell doesn't exist (%i,%i).\n", __FILE__, __LINE__, x, y);
        return NULL;
    }
    logToFile("INFO: value %s lies of coordinate %i %i\n", CELL_P(y, x).displayed_value, x, y);
    return &CELL_P(y, x);
}

// get the integer value of a cell given a coordinate value on the stack.
static int spreadsheetGetCell(Cellulose *client) {
    cell_t *cell = getCell(client);
    if (cell == NULL) {
        logToFile("%s: [%i] - Cannot access cell, cell is NULL\n", __FILE__, __LINE__);
        return -1;
    } else if (cell->cell_type != t_int) {
        logToFile("%s: [%i] - Cannot access cell, cell is not an integer it is type %i.\n", __FILE__, __LINE__, cell->cell_type);

    }

    return stackPush(cell->cell_value.number);
}

// Helper function for evalBCNum and evalBCCell. Evaluates the bytecode leaving the evaluated stack once finished.
static int evalBC(instructions_t *const bc, Cellulose* client, coord_int_t x, coord_int_t y) {
    assert(bc && "INVALID BYTECODE" );
    // "reset" the stack because we are evaluating new bytecode
    // all of the previous elements on the stack will be overwritten
    sp = 0;
    VEC_ITER((*bc), instruction_t, in) {
        switch (in.opcode) {
            case OP_PUSH: stackPush(in.opperand); break;
            case OP_PUSHX: stackPush(x); break;
            case OP_PUSHY: stackPush(y); break;
            case OP_GET_CELL: if (spreadsheetGetCell(client) == -2) { LOG_ERROR("%s", "ERROR: Could not find numerical cell value"); } break;
            OPCODE_CASE(OP_ADD, +)
            OPCODE_CASE(OP_DIV, /)
            OPCODE_CASE(OP_SUB, -)
            OPCODE_CASE(OP_MULT, *)
            default: return -1;
        }
    }
    // zero signifies a success
    return 0;
}

cell_num_t evalBCNum(instructions_t *byte_code, Cellulose *client, int *res, coord_int_t x, coord_int_t y) {
// argument byte_code is a pointer to instructions not an array pointer of instructions
    if (evalBC(byte_code, client, x, y) == -1) {
        *res = -1;
        return -1;
    }
    return stack[0];
}

cell_t* evalBCCell(instructions_t *byte_code, Cellulose *client, coord_int_t x, coord_int_t y) {
    if (evalBC(byte_code, client, x, y) != 0)
        return NULL;
    return getCell(client);
}
