// contains the default constructer and destructor of the struct Cellulose
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <ncurses.h>
#include <stdlib.h>
#include "../client.h"
#include "../config_include.h"

// takes in the client and renders its contents
// the entire client is needed and not just the spreadsheet because there is other info needed
// like the cursor position
void renderSpreadsheet(Cellulose *client, cursor_t* cursor);
// Renders a single cell given its coordinate. More efficient if redrawing the entire spreadsheet is not required.
void renderSingleCell(Cellulose* client, cursor_t* cursor, const size_t x, const size_t y);
// If redraw is true, draw the line that divedes the cells on the x-axis.
void drawSpreadsheetDividers(bool *redraw);

void drawColumnHeader(Cellulose *spreadsheet, cursor_t *cursor);
void drawRowHeader(Cellulose *spreadsheet, cursor_t *cursor);
