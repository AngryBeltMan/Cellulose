// use to repeat a command more than once
#include "../cursor/cursor_include.h"
#include <math.h>

#define REPEAT_CASES()\
    case '0': \
    case '1': \
    case '2': \
    case '3': \
    case '4': \
    case '5': \
    case '6': \
    case '7': \
    case '8': \
    case '9': { \
        cursor->repeat_count *= 10; \
        cursor->repeat_count += input - 48; \
    } break;

