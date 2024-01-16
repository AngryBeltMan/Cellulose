#include "cell.h"
#include "config_include.h"
#include "str.h"
inline void freeCell(cell_t cell) {
    if (cell.cell_type == t_empty)
        return;
    free(cell.displayed_value);
    if (cell.cell_type == t_str)
        free(cell.cell_value.str);
}

inline void print_cell(int x, int y, char *output, char *value) {
    move(y,x);
    memset(output, ' ', 10);
    output[11] = '\0';
    size_t len = strlen(value);
    memmove(output + 1, value, len < 11 ? len : 11);
    printw("%s%s", output,len > 12 ? " .." : "     ");
}

inline void create_cell(char *output, char *value, size_t value_len) {
    memset(output, ' ', 12);
    memcpy(output + 1, value, value_len < 12 ? value_len : 12);
    memcpy(output + 12, value_len > 12 ? ".." : "  ", 3);
}
// used for strToNum only
#define CURRENT_CHAR value[num_index - 1]

inline cell_num_t strToNum(char *value, size_t len) {
    cell_num_t converted_value = 0;
    float number_place = 1;
    for (size_t num_index = len; num_index > 0; --num_index, number_place *= 10) {
        if (CURRENT_CHAR == '.')
            number_place -= powf(10, len - num_index) - 0.1,
            converted_value /= powf(10, len - num_index);
        else
            converted_value += (value[num_index - 1] - 48) * number_place;
    }
    return converted_value;
}
