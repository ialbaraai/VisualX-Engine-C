cmake_minimum_required(VERSION 3.10)
project(core C)

add_library(core STATIC 
    src/core/core_string.c
    src/containers/core_vector.c
    src/containers/core_map.c
    src/core/core_file.c
)

set_target_properties(core PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
target_link_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)