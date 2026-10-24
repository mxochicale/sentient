cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach(src ${EXAMPLE_SOURCES})
    get_filename_component(name "${src}" NAME_WE)          # strip dir + .cpp
    add_executable(${name} ${src})                          # one binary per .cpp
    target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()
