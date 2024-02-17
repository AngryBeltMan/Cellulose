#include "vim_insert_mode.h"
#include "../messages.h"
#include "../formatting/parse_expr.h"
#include "../formatting/parse_bytecode.h"
#include <stdio.h>
#include <stdlib.h>
#define DELETE_KEY 127
// Error codes for errors that may occur when expanding
typedef enum __attribute__((__packed__)) {
    // no error occurred
    EXP_OK = 0,
    // invalid coordinate input ie negative values and zero values
    EXP_INV_COORD,
    // the given coordinate leads to an empty cell
    EXP_EMPTY_CELL,
    // The given coordinate leads to a cell that doesn't contain a numerical value. Error occurs when expanding equations.
    EXP_NON_NUM_CELL,
} expanding_error;

// Wrapper function for parseInputFormatted. Used to expand a the formatted str value based on a cell value with a given x and y value.
// Arguments:
// @client - Used to access the spreadsheet cells for expanding formatted input with a given cell value.
// @formin - The formatted str that is meant to be expanded.
// @formi - The index where the bracket of the soon to be expanded cell occured. The value of formi will be mutated so that, once the function has finished calling, formi will hold the index of the next char after the formatted specifier.
static int expandCellValue(Cellulose* client, const str* formin, size_t *form_i, str* expanded_input, instructions_t* ins, coord_int_t x, coord_int_t y) {
    // get the value after the left bracket
    ++*form_i;
    *ins = compileExpr(formin->contents, form_i);
    // the spreadsheet cell that will be expanded
    cell_t *cell = evalBCCell(ins, client, x + 1, y + 1);
    if (!cell) {
        LOG_ERROR("%s", "Failed to expand expression");
        return -1;
    }
    switch (cell->cell_type) {
        case t_str:
            appendCharArray(expanded_input, cell->cell_value.str, strlen(cell->cell_value.str)); break;
            break;
        case t_int:
            appendDouble(expanded_input, cell->cell_value.number); break;
        case t_empty: logToFile("37: Dis is empty\n"); break;
    }
    return 0;
}

// Wrapper function for parseInputFormatted. Used to evauate and expand and equation in a formatted string.
// Arguments:
// @client - Used to access the spreadsheet cells for expanding some of the cells numerical values.
// @formin - The formatted str that is meant to be expanded.
// @formi - The index where the bracket of the soon to be expanded cell occured. The value of formi will be mutated so that, once the function has finished calling, formi will hold the index of the end char after the formatted specifier.
// @ins - Pointer that will hold the vm instructions for the specified equation. This variable doesn't matter after the function is called and should deallocated as to avoid a memory leak.
// Before the function is called, the value of the variable ins can and should be NULL.
static int expandEquation(Cellulose* client, str* formin, size_t *form_i, str* expanded_input, instructions_t* ins, coord_int_t x, coord_int_t y) {
    int res = 0;
    ++*form_i;
    *ins = compileExpr(formin->contents, form_i);
    cell_num_t value = evalBCNum(ins, client, &res, x + 1, y + 1);
    // if evaluation failed don't append the value
    return res != -1 ? appendDouble(expanded_input, value) : -1;
}
// Wrapper function for parseInputFormatted. Used to parse the string and expand certain values.
// @client - Used to access the spreadsheet cells for expanding he formatted str.
// @formin - The formatted str that is meant to be expanded.
// @formi - The current index of arg formin.
// @expanded_input - The str that will be expanded.
// @ins - Pointer that will hold the vm instructions for the specified equation. This variable doesn't matter after the function is called and should deallocated as to avoid a memory leak.
// Before the function is called, the value of the variable ins can and should be NULL.
static int parseChar(Cellulose* client, str* formin, size_t *form_i, str* expanded_input, instructions_t* ins, coord_int_t x, coord_int_t y) {
    int res = 0;
    // check the previous char if it is special formatting char specifier '%'
    if (*form_i > 2 && formin->contents[*form_i - 1] == '%' ) {
        switch (formin->contents[*form_i]) {
            case 'x': res = appendNum(expanded_input, x); break;
            case 'y': res = appendNum(expanded_input, y); break;
            case '[': res = expandEquation(client, formin, form_i, expanded_input, ins, x, y); break;
            case '{': res = expandCellValue(client, formin, form_i, expanded_input, ins, x, y); break;
        }
    } else if (formin->contents[*form_i] != '%') {
        res = pushChar(expanded_input, formin->contents[*form_i] );
    }
    return res;
}

str_res parseInputFormatted(Cellulose *client, coord_int_t x, coord_int_t y, str *cell_input) {
    str_res expanded_str = strNew();
    instructions_t byte_code = {.elements = NULL};
    // return early with the error code
    if (expanded_str.result != str_no_error)
        return expanded_str;

    for (size_t char_index = 2; char_index < cell_input->len; ++char_index) {
        // if parsing fails clean up the expanded string and return a failure error code
        if (parseChar(client, cell_input, &char_index, &expanded_str.string, &byte_code, x, y))
            goto formatting_error;

        if (byte_code.elements)
            VEC_FREE(byte_code);
    }
    return expanded_str;
formatting_error:
    free(expanded_str.string.contents);
    if (byte_code.elements)
        VEC_FREE(byte_code);
    return (str_res) {.result = -1};
}

int setSelectedCells(Cellulose *client, coord_int_t x, coord_int_t y, bool exists, void *cell_input) {
    str* str_cell_input = (str*)cell_input;
    str input;
    if (str_cell_input->len > 2 && str_cell_input->contents[0] == 'f' && str_cell_input->contents[1] == '|') {
        str_res res = parseInputFormatted(client, x, y, str_cell_input);
        if (res.result == -1) return -1;
        input = res.string;
    } else {
        input = strClone(str_cell_input).string;
    }
    if (exists) { freeCell(CELL_P(y, x)); }
    setCell(client, x, y, &input );
    return 0;
}

// Wrapper function for parseCliInput. Pops the latest char typed in cell_in similar to pressing backspace on a text editor.
// Arguments:
// cell_in - The victim of the popping.
// client - Used to redraw the command line.
static inline void deleteChar(Cellulose *client, str *cell_in) {
     move(CLIENT_SHEET_HEIGHT + 1, 0);
     printw(COLUMN_HEADER_BG);
     client->redraw_cli = true;
     popFront(cell_in);
}

int parseCliInput(Cellulose *client, cursor_t *cursor, int input, str *cell_input) {
    int res = 0;
    switch (input) {
        case 10: {
            if (cursor->mode == COMMAND_MODE) {
                cursor->mode = NORMAL_MODE;
                res = runCommand(client, cursor, cell_input->contents);
                goto clean_up_input;
            }
            cursor->mode = NORMAL_MODE;
            // sets all of the cells selected to the inputed value
            if (cursor->visual_state != visual_state_NONE) {
                iterSelectedCells(client, cursor, &setSelectedCells, cell_input);
                client->redraw_spreadsheet = true;
                cursor->visual_state = visual_state_NONE;
                goto clean_up_input;
            }
            else {
            // checks to see if the cell being replaces had any heap allocated values
                if (cellExist(client, cursor->x, cursor->y ))
                    freeCell(CUR_CELL_P);
                setCell(client, cursor->x, cursor->y, cell_input);
                // don't clean up the input because the spreadsheet now has ownership of the value
                goto return_status;
            }
        } break;
        case DELETE_KEY: deleteChar(client, cell_input); goto return_status;
        // escape key
        case 27:
            cursor->mode = NORMAL_MODE;
            goto clean_up_input;
        default: {
            res = pushChar(cell_input, input);
            goto return_status;
        } break;
    }
clean_up_input:
    free(cell_input->contents);
    cell_input->contents = NULL;
return_status:
    return res;
}
