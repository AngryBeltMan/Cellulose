#include "../commands/cmd.h"
#include "../cursor/cursor_include.h"
#include "../client.h"
#include "../str.h"
#include "../config_include.h"
#include <curses.h>
//sets all of the selected cell to a certain value
int setSelectedCells(Cellulose *client, coord_int_t x, coord_int_t y, bool exists, void* cell_input);
// parses the input in cursor mode Command and Insert
int parseCliInput(Cellulose *client, cursor_t* cursor, int input, str *cell_input);

