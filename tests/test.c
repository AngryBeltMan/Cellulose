/* #include "tests.h" */
/* #include "csv_tests.c" */
/* #include "cell_tests.c" */
#include "../src/cell.h"
int main(void) {
    int input = 123;
    char output[10];
    intToStr(input, output);

    printf("%s\n", output);
    return 0;
}
