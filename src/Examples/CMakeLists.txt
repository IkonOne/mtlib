cmake_minimum_required(VERSION 3.8.0)

add_library(
    mtlib_examples_common
    common/performance_timer.cpp
)

add_executable(chull chull.cpp)
target_link_libraries(chull mtlib mtlib_examples_common)