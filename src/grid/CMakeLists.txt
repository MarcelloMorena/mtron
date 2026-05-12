cmake_minimum_required(VERSION 3.8)
project(grid)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(grid_interfaces REQUIRED)

add_executable(grid src/main.cpp src/Grid.cpp)
ament_target_dependencies(grid rclcpp std_msgs grid_interfaces)

target_include_directories(grid PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(grid PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

install(TARGETS grid
  DESTINATION lib/${PROJECT_NAME})

ament_package()
