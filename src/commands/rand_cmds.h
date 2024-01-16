#include "../client.h"
#include "../cursor/cursor.h"

int fillRandom(Cellulose* client, cursor_t* cursor, long double* args);

// generates a decimal number between a min and a max
int fillRandomRange(Cellulose* client, cursor_t* cursor, long double* args);

// generates a whole number between a min and a max
int fillRandomWholeRange(Cellulose* client, cursor_t* cursor, long double* args);

// suffles selected cells. Takes one argument in args. This argument specifies how many times the selected cells should be swapped.
int swapCellsCmd(Cellulose* client, cursor_t* cursor, long double* args);

// suffles selected cells. Takes one argument in args. This argument specifies how many times the selected cells should be swapped.
int shuffleCellsCmd(Cellulose* client, cursor_t* cursor, long double* args);
