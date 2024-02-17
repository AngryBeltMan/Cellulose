/* #include "../src/client.h" */
/* #include "csv_tests.c" */
#include "tests.h"
#include "formatting_tests.c"
#include "csv_tests.c"
#include "str_test.c"
#include "cell_tests.c"
#include "../src/messages.h"

/* #define CSV_TESTS_C */
#define FORMMATING_TESTS_C
/* #define STR_TEST_C */

int main(void) {
    initLogFile();
#ifdef CSV_TESTS_C
    RUN_TEST_NO_ARGS(csvTypeTest);
    RUN_TEST_NO_ARGS(many_rows_test);
    RUN_TEST_NO_ARGS(parse_large_cells);
    RUN_TEST_NO_ARGS(parse_multiple_rows);
    RUN_TEST_NO_ARGS(parse_single_row);
#endif
#ifdef FORMMATING_TESTS_C
    RUN_TEST_NO_ARGS(testCompilation)
    RUN_TEST_NO_ARGS(testParenthesis)
    RUN_TEST_NO_ARGS(testParenthesis2)
    RUN_TEST_NO_ARGS(testLongNums)
    RUN_TEST_NO_ARGS(testByteCodeAddition)
    RUN_TEST_NO_ARGS(testByteCodeDivision)
    RUN_TEST_NO_ARGS(testByteCodeParenthesis)
    RUN_TEST_NO_ARGS(testByteCodeParenthesis2)
    RUN_TEST_NO_ARGS(testDecimalAnswers)
    RUN_TEST_NO_ARGS(testVarX)
    RUN_TEST_NO_ARGS(testNoOpp)
    RUN_TEST_NO_ARGS(testCellGetParsing)
    RUN_TEST_NO_ARGS(testCellGetEvaluation)
    RUN_TEST_NO_ARGS(testNegativeResults)
    RUN_TEST_NO_ARGS(testExpandingCell)
    RUN_TEST_NO_ARGS(testExpandingSelectedCells)
#endif
#ifdef STR_TEST_C
    RUN_TEST_NO_ARGS(testWholeNumToStr);
    RUN_TEST_NO_ARGS(testDoubleToStrNegative);
    RUN_TEST_NO_ARGS(testStrAppendWholeNum);
    RUN_TEST_NO_ARGS(testIsNumNegative);
#endif
    PRINT_EVALUATION()
    freeLogFile();
    return 0;
}
