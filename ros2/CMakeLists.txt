cmake_minimum_required(VERSION 3.5)
project(openzen_driver)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(message_filters REQUIRED)

option(OPENZEN_ROS_BINARY_LIBRARIES "Download binary dependencies" OFF)

SET(ZEN_BLUETOOTH ON CACHE BOOL "Don't build bluetooth")
SET(ZEN_TESTS OFF CACHE BOOL "Don't build tests")
SET(ZEN_EXAMPLES OFF CACHE BOOL "Don't build examples")
SET(ZEN_USE_BINARY_LIBRARIES ${OPENZEN_ROS_BINARY_LIBRARIES} CACHE BOOL "Don't download binary dependencies (windows only)")
add_subdirectory(openzen)

## Build 
add_executable(openzen_node
  src/OpenZenNode.cpp
)

target_link_libraries(openzen_node
  OpenZen
)

ament_target_dependencies(openzen_node
  rclcpp
  std_msgs
  std_srvs
  sensor_msgs
  image_transport
  cv_bridge
  OpenCV)

install(TARGETS openzen_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/)

if (BUILD_TESTING)
  # launch_test is documented here:
  # https://github.com/ros2/launch/tree/master/launch_testing
  find_package(launch_testing_ament_cmake)
  add_launch_test(test/openzen_test.launch.py)
endif()

ament_package()
