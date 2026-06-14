cmake_minimum_required(VERSION 3.16)
project(engine C)

set(CMAKE_C_STANDARD 11)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)

set(TINYFD_DIR ${PROJECT_ROOT}/lib/tinyfiledialogs)
set(TINYFD_SRC ${TINYFD_DIR}/tinyfiledialogs.c)

set(NUKLEAR_DIR ${PROJECT_ROOT}/lib/nuklear)

set(LUAJIT_DIR ${PROJECT_ROOT}/lib/luajit)

set(CGLM_DIR ${PROJECT_ROOT}/lib/cglm)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    ${TINYFD_SRC}
    main.c
)

add_library(luajit STATIC IMPORTED)
set_target_properties(luajit PROPERTIES
    IMPORTED_LOCATION ${LUAJIT_DIR}/src/libluajit.a
)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/include
    ${TINYFD_DIR}
    ${NUKLEAR_DIR}
    ${LUAJIT_DIR}/src
    ${CGLM_DIR}
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/core lib/core)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    glfw
    Vulkan::Vulkan
    luajit
    dl
)