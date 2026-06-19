cmake_minimum_required(VERSION 3.16)
project(engine)

set(CMAKE_C_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

set(PROJECT_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)

set(TINYFD_DIR ${PROJECT_ROOT}/lib/tinyfiledialogs)
set(TINYFD_SRC ${TINYFD_DIR}/tinyfiledialogs.c)

set(STB_DIR ${PROJECT_ROOT}/lib/stb)

set(NUKLEAR_DIR ${PROJECT_ROOT}/lib/nuklear)
set(LUAJIT_DIR ${PROJECT_ROOT}/lib/luajit)
set(CGLM_DIR ${PROJECT_ROOT}/lib/cglm)
set(GLAD_DIR ${PROJECT_ROOT}/lib/glad)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_library(glad STATIC
    ${GLAD_DIR}/src/gl.c
)
target_include_directories(glad PUBLIC
    ${GLAD_DIR}/include
)

add_library(luajit STATIC IMPORTED)
set_target_properties(luajit PROPERTIES
    IMPORTED_LOCATION ${LUAJIT_DIR}/src/libluajit.a
    INTERFACE_INCLUDE_DIRECTORIES ${LUAJIT_DIR}/src
)

find_package(OpenGL REQUIRED)

add_subdirectory(${PROJECT_ROOT}/lib/core lib/core)

add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    ${TINYFD_SRC}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/core/include
    ${TINYFD_DIR}
    ${NUKLEAR_DIR}
    ${CGLM_DIR}
    ${GLAD_DIR}/include
    ${GLAD_DIR}/src
    ${STB_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
    glfw
    OpenGL::GL
    glad
    luajit
)

if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE dl m)
endif()

add_custom_command(TARGET engine 
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ROOT}/assets
    $<TARGET_FILE_DIR:engine>/assets
)