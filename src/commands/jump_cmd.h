// Jump commands differ from repeated cursor movement (example being '10j' or '12k'), as those commands move relative to the cursor position. On the other hand, these commands move to the specified coordinate instead.
#include "../client.h"
#include "../cursor/cursor_include.h"
// jump to a specific column
int commandJumpX(Cellulose* client, cursor_t* cursor, long double *args);

// jump to a specific row
int commandJumpY(Cellulose* client, cursor_t* cursor, long double *args);

// jump to a specific cell coordinate
int commandJumpXY(Cellulose* client, cursor_t* cursor, long double *args);

