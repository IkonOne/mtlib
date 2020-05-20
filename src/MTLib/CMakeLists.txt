add_subdirectory(comp_geo)

add_library(mtlib)
target_link_libraries(mtlib ${3RDPARTY_LIBS} comp_geo)