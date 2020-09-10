add_subdirectory(algebra)
add_subdirectory(comp_geo)
add_subdirectory(ds)
add_subdirectory(geometry)
add_subdirectory(intersection)

add_library(mtlib INTERFACE)
# target_link_libraries(mtlib
#     ${3RDPARTY_LIBS}
#     # mtlib_algebra
#     # mtlib_comp_geo
#     # mtlib_geometry
# )