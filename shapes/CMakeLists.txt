set(src_dir ${CMAKE_CURRENT_SOURCE_DIR})

# Add overall src library.
file(GLOB_RECURSE shapes_sources CONFIGURE_DEPENDS ${shapes_dir}/*.cpp)
add_library(shapes ${shapes_sources})
target_include_directories(shapes PUBLIC ${shapes_dir})