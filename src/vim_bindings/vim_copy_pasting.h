#pragma once
#include "../cursor/cursor_include.h"
#include "../client.h"
#include "vim_cursor_movement.h"

#define COPY_PASTE_CASES() \
        case 'd':  parseKeyDY(client, cursor, 'd'); break;  \
        case 'y':  parseKeyDY(client, cursor, 'y'); break;  \
        case 'p':  parseKeyP(client, cursor);  break;

int addCellToClipboard(cursor_t* cursor, cell_t cell, size_t clipboard_row);

int deleteCells(Cellulose *client,unsigned short x, unsigned short y, bool exist, void* arg_cursor);

int copyCells(Cellulose *client,unsigned short x, unsigned short y, bool exist, void* arg_cursor);
// parses either key d or y cause they are very similar
int parseKeyDY(Cellulose* client, cursor_t* cursor, char key);

void parseKeyP(Cellulose* client, cursor_t* cursor);

