#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// boolean value that checks if all of the tests passed
static int FAILED = 0;
static int PASSED = 0;
#define T_ASSERT(_cmp) if (!(_cmp)) {fprintf(stderr, "\033[38;2;255;25;0mAssertion Failed: (comparison: %s, function: %s, file: %s, line number: %i)\033[0m\n", #_cmp, __FUNCTION__, __FILE__, __LINE__); return T_FAILURE;}
#define T_SUCCESS 1
#define T_FAILURE 0
#define RUN_TEST(test_fn, ...) if (test_fn(__VA_ARGS__)) { \
    printf("\033[38;2;0;255;25mRESULT: FUNCTION %s PASSED\033[0m\n", #test_fn); \
    ++PASSED;\
} else { \
    printf("\033[38;2;255;25;0mRESULT: FUNCTION %s FAILED\033[0m\n", #test_fn); ALL_PASSED = 0;\
    ++FAILED;\
}

#define RUN_TEST_NO_ARGS(test_fn) if (test_fn()) { \
    printf("\033[38;2;0;255;25mRESULT: FUNCTION %s PASSED\033[0m\n", #test_fn); \
    ++PASSED;\
} else { \
    printf("\033[38;2;255;25;0mRESULT: FUNCTION %s FAILED\033[0m\n", #test_fn);\
    ++FAILED;\
}
#define PRINT_EVALUATION() \
    if (FAILED == 0) { \
        printf("\033[38;2;0;255;25mEVALUATION: %i out of %i tests passed\033[0m\n", PASSED, PASSED); \
    } else { \
        printf("\033[38;2;255;25;0mEVALUATION: %i out of %i tests passed\033[0m\n", PASSED, PASSED + FAILED );\
    }
