#!/bin/sh
INPUT_FILE="../test.csv";
cmake_build() {
    cd build
    make
}
valgrind_mode() {
    cmake_build
    echo "running using valgrind"
    valgrind --leak-check=full ./cellulose $INPUT_FILE
}
test_mode() {
    echo "running all of the tests"
    gcc -g -Wall -Wextra -Wpedantic -o tests.out tests/test.c -lncurses -lm
    ./tests.out
}
run_normal() {
    cmake_build
    ./cellulose $INPUT_FILE
}

# get command line flags
# execute_mode will be used to see how to execute the executable
getopts ":v :t" execute_mode
case "${execute_mode}" in
    v) valgrind_mode ;;
    t) test_mode ;;
    *) run_normal ;;
esac
