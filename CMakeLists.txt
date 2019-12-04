cmake_minimum_required(VERSION 3.5.0)

project(marker_test)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)

add_executable(marker_test main.cpp)

target_link_libraries(marker_test
  PRIVATE
    ${rclcpp_LIBRARIES}
    ${visualization_msgs_LIBRARIES}
)

target_include_directories(marker_test
  PRIVATE
    ${rclcpp_INCLUDE_DIRS}
    ${visualization_msgs_INCLUDE_DIRS}
)
