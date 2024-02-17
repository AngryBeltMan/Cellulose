#include "messages.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
static FILE* ERROR_FILE;
void logToFile(char *fmt, ...) {
    va_list vargs;
    va_start(vargs, fmt);
    vfprintf(ERROR_FILE, fmt, vargs);
    va_end(vargs);
}
void initLogFile() {
    ERROR_FILE = fopen("cellulose_log.txt", "w");
    assert(ERROR_FILE && "Failed to open log file");
}

void freeLogFile() {
    fclose(ERROR_FILE);
}
