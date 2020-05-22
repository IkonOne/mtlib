add_subdirectory(comp_geo)
add_subdirectory(algebra)

add_library(mtlib)
target_link_libraries(mtlib
    ${3RDPARTY_LIBS}
    mtlib_comp_geo
    mtlib_algebra
)