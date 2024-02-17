#include "../src/formatting/parse_expr.h"
#include "../src/formatting/parse_bytecode.h"
#include "../src/parsing.h"
#include "tests.h"
#include <stdio.h>
int testCompilation() {
    char* expr = "[2+3-2]";
    size_t index = 1;
    instructions_t res = compileExpr(expr, &index);
    T_ASSERT(res.elements != NULL);
    T_ASSERT(res.length == 5);
    T_ASSERT(res.elements[0].opcode == OP_PUSH);
    T_ASSERT(res.elements[1].opcode == OP_PUSH);
    T_ASSERT(res.elements[2].opcode == OP_ADD);

    T_ASSERT(res.elements[3].opcode == OP_PUSH);
    T_ASSERT(res.elements[4].opcode == OP_SUB);
    VEC_FREE(res);
    return T_SUCCESS;
}
// 1+(2+3)
int testParenthesis() {
    char* expr = "[1+(2+3)]";
    size_t index = 1;
    instructions_t res = compileExpr(expr, &index);
    T_ASSERT(res.elements != NULL);
    T_ASSERT(res.length == 5);
    T_ASSERT(res.elements[0].opcode == OP_PUSH);
    T_ASSERT(res.elements[1].opcode == OP_PUSH);
    T_ASSERT(res.elements[2].opcode == OP_PUSH);
    T_ASSERT(res.elements[3].opcode == OP_ADD);
    T_ASSERT(res.elements[4].opcode == OP_ADD);
    VEC_FREE(res);
    return T_SUCCESS;
}

int testLongNums() {
    char* expr = "[10+20]";
    size_t index = 1;
    instructions_t res = compileExpr(expr, &index);
    T_ASSERT(res.elements != NULL);
    T_ASSERT(res.length == 3);
    T_ASSERT(res.elements[0].opcode == OP_PUSH);
    T_ASSERT(res.elements[1].opcode == OP_PUSH);
    T_ASSERT(res.elements[2].opcode == OP_ADD);

    T_ASSERT(res.elements[0].opperand == 10);
    T_ASSERT(res.elements[1].opperand == 20);

    int err_code = 0;
    cell_num_t val = evalBCNum(&res, NULL, &err_code, 0, 0 );
    T_ASSERT(val == 30)
    VEC_FREE(res);
    return T_SUCCESS;
}
int testCellGetParsing() {
    char* expr = "[(1,2)]";
    size_t index = 1;
    instructions_t res = compileExpr(expr, &index);
    T_ASSERT(res.elements != NULL);
    T_ASSERT(res.length == 3);
    T_ASSERT(res.elements[0].opcode == OP_PUSH);
    T_ASSERT(res.elements[1].opcode == OP_PUSH);
    printf("opcode %i\n", res.elements[2].opcode);
    T_ASSERT(res.elements[2].opcode == OP_GET_CELL);
    T_ASSERT(res.elements[0].opperand == 1);
    T_ASSERT(res.elements[1].opperand == 2);
    VEC_FREE(res);
    return T_SUCCESS;

}
int testParenthesis2() {
    char* expr = "[(3+4)+(2+3)]";
    size_t index = 1;
    instructions_t res = compileExpr(expr, &index);
    T_ASSERT(res.elements != NULL);
    T_ASSERT(res.length == 7);
    T_ASSERT(res.elements[0].opcode == OP_PUSH);
    T_ASSERT(res.elements[1].opcode == OP_PUSH);
    T_ASSERT(res.elements[2].opcode == OP_ADD);
    T_ASSERT(res.elements[3].opcode == OP_PUSH);
    T_ASSERT(res.elements[4].opcode == OP_PUSH);
    T_ASSERT(res.elements[5].opcode == OP_ADD);
    T_ASSERT(res.elements[6].opcode == OP_ADD);
    VEC_FREE(res);
    return T_SUCCESS;

}
int testByteCodeAddition() {
    char* expr = "[2+3]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, NULL, &res, 0, 0 );
    T_ASSERT(res == 0);
    T_ASSERT(value == 5);
    VEC_FREE(bc);
    return T_SUCCESS;
}

int testByteCodeDivision() {
    char* expr = "[2/2]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, NULL, &res, 0, 0 );
    T_ASSERT(res == 0);
    T_ASSERT(value == 1);
    VEC_FREE(bc);
    return T_SUCCESS;
}
int testByteCodeParenthesis() {
    char* expr = "[1+(2+3)]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, NULL, &res, 0, 0 );
    T_ASSERT(res == 0);
    T_ASSERT(value == 6);
    VEC_FREE(bc);
    return T_SUCCESS;
}
int testByteCodeParenthesis2() {
    char* expr = "[2+(4/2)]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, NULL, &res, 0, 0 );
    T_ASSERT(res == 0);
    T_ASSERT(value == 4);
    VEC_FREE(bc);
    return T_SUCCESS;

}

// Divides to values so that the result is a decimal
int testDecimalAnswers() {
    char* expr = "[5/2]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, NULL, &res, 0, 0);
    T_ASSERT(res == 0);
    T_ASSERT(value == 2.5);
    VEC_FREE(bc);
    return T_SUCCESS;

}
// test if var x can be evauated properly
int testVarX() {
    char *expr = "[x-2]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    T_ASSERT(bc.elements[0].opcode == OP_PUSHX );
    T_ASSERT(bc.elements[1].opperand == 2 );
    VEC_FREE(bc);
    return T_SUCCESS;
}
// tests expressions with just a single value no opperations
int testNoOpp() {
    char *expr = "[23]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, NULL, &res , 1, 1);
    T_ASSERT(res == 0);
    T_ASSERT(value == 23);
    VEC_FREE(bc);
    return T_SUCCESS;
}

int testNegativeResults() {
    char* expr = "[2-4]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    T_ASSERT(bc.length == 3);
    T_ASSERT(bc.elements[0].opcode == OP_PUSH);
    T_ASSERT(bc.elements[1].opcode == OP_PUSH);
    T_ASSERT(bc.elements[2].opcode == OP_SUB);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, NULL, &res, 0, 0 );
    T_ASSERT(value == -2);
    T_ASSERT(res == 0);
    VEC_FREE(bc);
    return T_SUCCESS;

}

int testCellGetEvaluation() {
    Cellulose client = newEmpty("tests/csvs/formatting_test.csv");
    fromCSV(&client);
    char* expr = "[(1,2)+3]";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    T_ASSERT(bc.elements != NULL);
    int res = 0;
    // value of the evaluated compiled expr
    cell_num_t value = evalBCNum(&bc, &client, &res , 0, 0);
    printf("val: %f\n", value);
    T_ASSERT(res == 0);
    T_ASSERT(value == 7);
    VEC_FREE(bc);
    return T_SUCCESS;
}

int testExpandingCell() {
    Cellulose client = newEmpty("tests/csvs/formatting_test.csv");
    fromCSV(&client);
    char *expr = "{1:1}";
    size_t index = 1;
    instructions_t bc = compileExpr(expr, &index);
    cell_t *cell = evalBCCell(&bc, &client, 0, 0);
    T_ASSERT(cell);
    T_ASSERT(cell->cell_type == t_int);
    T_ASSERT(cell->cell_value.number == 1.0);
    T_ASSERT(index == 4);
    freeSpreadsheet(client);
    return T_SUCCESS;
}
int testExpandingSelectedCells() {
    const cell_num_t results[] = { 2,5,8 };
    Cellulose client = newEmpty("tests/csvs/formatting_test.csv");
    fromCSV(&client);
    char *expr = "[(1,y)+1]";
    for (int y = 1; y < 4; y++) {
        size_t i = 1;
        instructions_t ins = compileExpr(expr, &i);
        int res = 0;
        cell_num_t value = evalBCNum(&ins, &client, &res, y, y);
        printf("v %f\n", value);
        T_ASSERT(res == 0);
        T_ASSERT(value == results[y - 1]);
        VEC_FREE(ins);
    }
    freeSpreadsheet(client);
    return T_SUCCESS;
}
