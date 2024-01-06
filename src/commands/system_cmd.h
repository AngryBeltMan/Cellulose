// Contains a list of commands that don't mutate the spreadsheet itself
#include "../client.h"
#include "../cursor/cursor_include.h"

// exits the spreadsheet
int commandExit(Cellulose* client, cursor_t* cursor, long double* args);

// saves the contents of the spreadsheet to a cetain file
int commandSave(Cellulose* client, cursor_t* cursor, long double* args);

// redraws the cells and the cli
int commandRefresh(Cellulose* client, cursor_t* cursor, long double* args);
