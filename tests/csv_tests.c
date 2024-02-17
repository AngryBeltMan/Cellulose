#include "../src/parsing.h"
#include "../src/client.h"
#include "tests.h"
#include <string.h>

int parse_single_row() {
    Cellulose client = newEmpty("tests/csvs/single_row.csv");
    fromCSV(&client);
    T_ASSERT(client.spread_sheet.length == 1);
    T_ASSERT(ELEMENTS[0].length == 4);
    // not opperator because strcmp returns 0 or false if the strings match
    T_ASSERT(!strcmp(CELL_VAL(0, 0), "only"));
    T_ASSERT(!strcmp(CELL_VAL(0, 1), "a"));
    T_ASSERT(!strcmp(CELL_VAL(0, 2), "single"));
    T_ASSERT(!strcmp(CELL_VAL(0, 3), "row"));
    freeSpreadsheet(client);
    return T_SUCCESS;
}
int parse_multiple_rows() {
    Cellulose client = newEmpty("tests/csvs/multiple_lines.csv");
    fromCSV(&client);
    T_ASSERT(SHEET.length == 3);
    T_ASSERT(ELEMENTS[0].length == 3);
    T_ASSERT(ELEMENTS[1].length == 3);
    T_ASSERT(ELEMENTS[2].length == 2);
    // not opperator because strcmp returns 0 or false if the strings match
    T_ASSERT(!strcmp(CELL_VAL(0,0), "line"));
    T_ASSERT(!strcmp(CELL_VAL(0,1), "number"));
    T_ASSERT(!strcmp(CELL_VAL(0,2), "one"));

    // line two
    T_ASSERT(!strcmp(CELL_VAL(1,0), "line"));
    T_ASSERT(!strcmp(CELL_VAL(1,1), "numero"));
    T_ASSERT(!strcmp(CELL_VAL(1,2), "dos"));

    // line three
    T_ASSERT(!strcmp(CELL_VAL(2,0), "third"));
    T_ASSERT(!strcmp(CELL_VAL(2,1), "line"));
    freeSpreadsheet(client);
    return T_SUCCESS;
}
// tests to see if longer cell (longer than six characters)values works without any undefined behavior
int parse_large_cells() {
    Cellulose client = newEmpty("tests/csvs/long_cells.csv");
    fromCSV(&client);
    T_ASSERT(!strcmp(CELL_VAL(0,0), "extralongcell"));
    T_ASSERT(!strcmp(CELL_VAL(0,1), "longerthan"));
    T_ASSERT(!strcmp(CELL_VAL(0,2), "sixchars"));
    freeSpreadsheet(client);
    return T_SUCCESS;
}

// tests to see if many rows (more than five rows) works without any undefined behavior
int many_rows_test() {
    Cellulose client = newEmpty("tests/csvs/lots_of_lines.csv");
    fromCSV(&client);
    VEC_ITER(client.spread_sheet, row_t, current_row) {
        VEC_ITER(current_row, cell_t, current_cell) {
            T_ASSERT(!strcmp(current_cell.cell_value.str, "hi"));
        }
    }
    freeSpreadsheet(client);
    return T_SUCCESS;
}
int csvTypeTest() {
    Cellulose client = newEmpty("tests/csvs/int_and_str.csv");
    fromCSV(&client);
    T_ASSERT(CELL(0,0).cell_type == t_str);
    T_ASSERT(CELL(0,1).cell_type == t_int);
    T_ASSERT(CELL(0,2).cell_type == t_int);
    T_ASSERT(CELL(0,3).cell_type == t_str);
    T_ASSERT(CELL(0,4).cell_type == t_str);
    T_ASSERT(CELL(0,5).cell_type == t_int);
    freeSpreadsheet(client);
    return T_SUCCESS;
}
