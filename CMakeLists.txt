cmake_minimum_required(VERSION 3.8.0)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_EXAMPLES "Build mtlib examples if on." ON)
option(BUILD_TESTS "Build mtlib unit tests if on." ON)

project(mtlib VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Configuring mtlib ${PROJECT_VERSION}")

add_subdirectory(3rdparty)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(
    SYSTEM
    ${3RD_PARTY_INCLUDE_DIRS}
)

add_subdirectory(src)