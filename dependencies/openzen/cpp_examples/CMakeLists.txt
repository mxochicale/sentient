cmake_minimum_required(VERSION 3.5)
project(example-cpp VERSION 0.1.0 LANGUAGES CXX;C)

set (CMAKE_PREFIX_PATH $ENV{HOME}/repositories/openzen/build)

find_package(OpenZen REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}14 example-cpp14.cpp)
add_executable(${CMAKE_PROJECT_NAME}17 example-cpp17.cpp)

set_target_properties(${CMAKE_PROJECT_NAME}14 PROPERTIES CXX_STANDARD 14)
set_target_properties(${CMAKE_PROJECT_NAME}17 PROPERTIES CXX_STANDARD 17)

target_link_libraries(${CMAKE_PROJECT_NAME}14 OpenZen::OpenZen)
target_link_libraries(${CMAKE_PROJECT_NAME}17 OpenZen::OpenZen)
