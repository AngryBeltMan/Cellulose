cmake_minimum_required(VERSION 3.19)
project(cellulose)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
# the path to a file that includes information on how the spreadsheet should be look, macros, plugins, etc
set(CONFIG_DIR ../config.h)

# use the default a default c compiler path unless it is provided a different one via the CLANG_DIR enviorment variable
if(EXISTS ${CLANG_DIR})
    message(STATUS "INFO: using user provided C compiler path: ${CLANG_DIR}.")
    set(CMAKE_C_COMPILER ${CLANG_DIR})
else()
    message(STATUS "INFO: using default C compiler path: /bin/clang")
    set(CMAKE_C_COMPILER /bin/clang)
endif()

if(EXISTS $ENV{CELLULOSE_CONFIG_DIR})
    message(STATUS "INFO: using custom config path: $ENV{CELLULOSE_CONFIG_DIR}.")
    set(CONFIG_DIR $ENV{CELLULOSE_CONFIG_DIR})
else()
    message(STATUS "INFO: using default config path: ${DEFAULT_CONFIG_DIR}.")
endif()

add_executable(cellulose src/main.c)
target_link_libraries(cellulose PUBLIC ncurses)
# link the math library
target_link_libraries(cellulose PUBLIC m)
target_compile_options(cellulose PRIVATE -Wall -Wextra -g)
add_compile_definitions(CONFIG_PATH="${CONFIG_DIR}")
