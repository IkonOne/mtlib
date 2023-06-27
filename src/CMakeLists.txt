cmake_minimum_required(VERSION 3.8.0)

add_subdirectory(MTLib)

if (BUILD_TESTS)
    add_subdirectory(Tests)
endif(BUILD_TESTS)

if (BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif(BUILD_EXAMPLES)