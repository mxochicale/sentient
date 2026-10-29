cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(OpenZen REQUIRED)

# Pure utility code, no OpenZen dependency — reusable by tests.
add_library(imu_utils STATIC imu_utils.cpp)
target_include_directories(imu_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(imu_utils PROPERTIES CXX_STANDARD 17)

# The hardware-facing executable.
add_executable(${CMAKE_PROJECT_NAME}17 main.cpp)
set_target_properties(${CMAKE_PROJECT_NAME}17 PROPERTIES CXX_STANDARD 17)
target_link_libraries(${CMAKE_PROJECT_NAME}17 PRIVATE imu_utils OpenZen::OpenZen)

# Tests (only if GTest is available).
find_package(GTest QUIET)
if (GTest_FOUND)
    enable_testing()
    add_executable(test_imu_utils test_imu_utils.cpp)
    set_target_properties(test_imu_utils PROPERTIES CXX_STANDARD 17)
    target_link_libraries(test_imu_utils PRIVATE imu_utils GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(test_imu_utils)
endif()