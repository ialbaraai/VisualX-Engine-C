cmake_minimum_required(VERSION 3.16)
project(engine C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    main.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib/core lib/core)

target_link_libraries(${PROJECT_NAME} PRIVATE
    core
)