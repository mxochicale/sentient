cmake_minimum_required(VERSION 3.5)

get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(OpenZen REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}17 main.cpp)
set_target_properties(${CMAKE_PROJECT_NAME}17 PROPERTIES CXX_STANDARD 17)
target_link_libraries(${CMAKE_PROJECT_NAME}17 OpenZen::OpenZen)
