include_directories(".")
file(GLOB_RECURSE UNIT_TEST_SOURCE_FILES "*.cpp")

add_compile_options(-Wall -Wpedantic -Wextra)
add_compile_options(-Wno-unused-local-typedefs)
add_compile_options(-Wno-ignored-qualifiers)
add_compile_options(-ggdb)
add_executable(unit_test ${UNIT_TEST_SOURCE_FILES} comp_geo/half_edge_list_2d_unittest.cpp)
target_link_libraries(unit_test mtlib gtest_main ${3RD_PARTY_LIBS})