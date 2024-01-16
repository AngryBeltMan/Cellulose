#include "config_include.h"
// Displays output for the user on the bottom of the screen
#define LOG_INFO(_fmt, ...) \
    move(CLIENT_SHEET_HEIGHT + 1, 0); \
    printw(COLUMN_HEADER_BG);\
    move(CLIENT_SHEET_HEIGHT + 1, 1); \
    attron(COLOR_PAIR(INFO_MSG_ID)); \
    printw(_fmt, __VA_ARGS__);\


#define LOG_ERROR(_fmt, ...) \
    move(CLIENT_SHEET_HEIGHT + 1, 0); \
    printw(COLUMN_HEADER_BG);\
    move(CLIENT_SHEET_HEIGHT + 1, 1); \
    attron(COLOR_PAIR(ERROR_MSG_ID)); \
    printw(_fmt, __VA_ARGS__);
