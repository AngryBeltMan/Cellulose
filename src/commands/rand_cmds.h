#include "../client.h"
#include "../cursor/cursor.h"

int fillRandom(Cellulose* client, cursor_t* cursor, long double* args);

// generates a decimal number between a min and a max
int fillRandomRange(Cellulose* client, cursor_t* cursor, long double* args);

// generates a whole number between a min and a max
int fillRandomWholeRange(Cellulose* client, cursor_t* cursor, long double* args);
